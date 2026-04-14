"""
Chapter 10: Learning and Adaptation Pattern - Direct Preference Optimization (DPO)
(学习与适应模式 - 直接偏好优化)

DPO 是一种革命性的对齐方法，它直接从人类偏好数据中学习，无需训练单独的奖励模型。
相比传统的 RLHF (PPO) 方法，DPO 更简单、更稳定、更高效。

核心概念：
- **偏好数据 (Preference Data)**: 成对的 (prompt, chosen, rejected) 数据
- **Bradley-Terry 模型**: 建模人类偏好的概率分布
- **闭式解**: 将 RL 问题转化为简单的分类损失

DPO 公式：
L_DPO = -log σ(β * log(π_θ(y_w|x) / π_ref(y_w|x)) - β * log(π_θ(y_l|x) / π_ref(y_l|x)))

其中：
- π_θ: 当前策略（待优化模型）
- π_ref: 参考策略（通常是 SFT 模型）
- y_w: 人类偏好的回答 (chosen)
- y_l: 人类不喜欢的回答 (rejected)
- β: 温度系数，控制与参考策略的偏离程度

本示例实现：
1. 基于 LLM API 的 DPO 训练模拟
2. 偏好数据收集与管理
3. 策略更新与评估
4. 完整的 DPO 训练流程演示
"""

import os
import json
import random
from typing import List, Dict, Tuple, Optional, Any
from dataclasses import dataclass, field, asdict
from datetime import datetime
from enum import Enum
import numpy as np
from langchain_openai import ChatOpenAI
from langchain_core.prompts import ChatPromptTemplate
from langchain_core.output_parsers import StrOutputParser
from dotenv import load_dotenv

load_dotenv()


# ============ 1. 数据结构与配置 ============

class PreferenceLabel(Enum):
    """偏好标签"""
    CHOSEN = "chosen"      # 偏好的回答
    REJECTED = "rejected"  # 不喜欢的回答
    NEUTRAL = "neutral"    # 中性/同等偏好


@dataclass
class PreferencePair:
    """
    偏好对数据结构
    
    包含：
    - prompt: 输入提示
    - chosen: 偏好的回答
    - rejected: 不喜欢的回答
    - preference_strength: 偏好强度 (0-1)
    """
    prompt: str
    chosen: str
    rejected: str
    id: str = field(default_factory=lambda: f"pp_{datetime.now().strftime('%Y%m%d%H%M%S')}_{random.randint(1000, 9999)}")
    timestamp: str = field(default_factory=lambda: datetime.now().isoformat())
    preference_strength: float = 1.0  # 偏好强度，1.0 表示非常确定
    metadata: Dict[str, Any] = field(default_factory=dict)
    
    def to_dict(self) -> Dict:
        return {
            "id": self.id,
            "prompt": self.prompt,
            "chosen": self.chosen,
            "rejected": self.rejected,
            "preference_strength": self.preference_strength,
            "timestamp": self.timestamp,
            "metadata": self.metadata
        }


@dataclass
class DPOConfig:
    """DPO 训练配置"""
    beta: float = 0.1                    # 温度系数，控制偏离参考策略的程度
    learning_rate: float = 1e-5          # 学习率
    batch_size: int = 4                  # 批次大小
    num_epochs: int = 3                  # 训练轮数
    max_grad_norm: float = 1.0           # 梯度裁剪
    reference_model_temperature: float = 0.0  # 参考模型温度
    policy_model_temperature: float = 0.7     # 策略模型温度
    
    def __post_init__(self):
        assert self.beta > 0, "beta 必须为正数"
        assert 0 < self.learning_rate < 1, "学习率应在 (0, 1) 之间"


# ============ 2. 偏好数据管理 ============

class PreferenceDataset:
    """
    偏好数据集管理器
    
    功能：
    - 存储和加载偏好对
    - 数据增强和过滤
    - 批量采样
    """
    
    def __init__(self):
        self.pairs: List[PreferencePair] = []
        
    def add_pair(self, pair: PreferencePair):
        """添加偏好对"""
        self.pairs.append(pair)
        
    def add_from_comparison(self, prompt: str, response_a: str, response_b: str, 
                           preferred: str, strength: float = 1.0):
        """
        从比较结果添加偏好对
        
        Args:
            prompt: 输入提示
            response_a: 回答 A
            response_b: 回答 B
            preferred: "A" 或 "B"，表示偏好哪个
            strength: 偏好强度
        """
        if preferred == "A":
            pair = PreferencePair(
                prompt=prompt,
                chosen=response_a,
                rejected=response_b,
                preference_strength=strength
            )
        else:
            pair = PreferencePair(
                prompt=prompt,
                chosen=response_b,
                rejected=response_a,
                preference_strength=strength
            )
        self.add_pair(pair)
        
    def get_batch(self, batch_size: int, shuffle: bool = True) -> List[PreferencePair]:
        """获取批次数据"""
        if shuffle:
            batch = random.sample(self.pairs, min(batch_size, len(self.pairs)))
        else:
            batch = self.pairs[:batch_size]
        return batch
    
    def get_all(self) -> List[PreferencePair]:
        """获取所有数据"""
        return self.pairs
    
    def __len__(self) -> int:
        return len(self.pairs)
    
    def save(self, filepath: str):
        """保存到文件"""
        data = [p.to_dict() for p in self.pairs]
        with open(filepath, 'w', encoding='utf-8') as f:
            json.dump(data, f, ensure_ascii=False, indent=2)
    
    def load(self, filepath: str):
        """从文件加载"""
        with open(filepath, 'r', encoding='utf-8') as f:
            data = json.load(f)
        self.pairs = []
        for item in data:
            pair = PreferencePair(
                prompt=item["prompt"],
                chosen=item["chosen"],
                rejected=item["rejected"],
                id=item.get("id", ""),
                timestamp=item.get("timestamp", ""),
                preference_strength=item.get("preference_strength", 1.0),
                metadata=item.get("metadata", {})
            )
            self.pairs.append(pair)


# ============ 3. DPO 训练器 ============

class DPOTrainer:
    """
    DPO (Direct Preference Optimization) 训练器
    
    使用 LLM API 模拟 DPO 训练过程：
    1. 计算参考策略 π_ref 的 log-prob
    2. 计算当前策略 π_θ 的 log-prob
    3. 计算 DPO 损失
    4. 更新策略（通过 prompt 工程模拟）
    
    注意：这是一个概念演示，真实的 DPO 需要模型权重访问。
    这里使用 LLM API 和提示词工程来模拟训练效果。
    """
    
    def __init__(self, config: DPOConfig = None):
        self.config = config or DPOConfig()
        
        # 参考模型（固定，通常是 SFT 后的模型）
        self.reference_llm = ChatOpenAI(
            model="deepseek-chat",
            temperature=self.config.reference_model_temperature,
            base_url="https://api.deepseek.com/v1",
            api_key=os.getenv("DEEPSEEK_API_KEY")
        )
        
        # 策略模型（待优化，温度更高以增加探索）
        self.policy_llm = ChatOpenAI(
            model="deepseek-chat",
            temperature=self.config.policy_model_temperature,
            base_url="https://api.deepseek.com/v1",
            api_key=os.getenv("DEEPSEEK_API_KEY")
        )
        
        # 训练历史
        self.training_history: List[Dict] = []
        
    def compute_log_prob(self, prompt: str, response: str, is_reference: bool = True) -> float:
        """
        计算给定 prompt 下生成 response 的对数概率
        
        由于 API 通常不直接提供 log-prob，我们使用启发式方法估计：
        - 让模型评估 "这个回答有多好"
        - 转换为概率估计
        
        Args:
            prompt: 输入提示
            response: 生成的回答
            is_reference: 是否使用参考模型
        
        Returns:
            估计的对数概率
        """
        llm = self.reference_llm if is_reference else self.policy_llm
        
        eval_prompt = f"""评估以下回答在给定提示下的合理性。

提示: {prompt}

回答: {response}

请评估这个回答的质量（0-10分），只返回数字。"""
        
        try:
            result = llm.invoke(eval_prompt)
            score_text = result.content.strip()
            # 提取数字
            import re
            numbers = re.findall(r'\d+\.?\d*', score_text)
            if numbers:
                score = float(numbers[0])
                # 转换为对数概率（归一化到 [-10, 0] 范围）
                log_prob = (score / 10) * 10 - 10
                return log_prob
        except Exception as e:
            print(f"计算 log-prob 时出错: {e}")
        
        # 默认返回中等概率
        return -5.0
    
    def compute_dpo_loss(self, pair: PreferencePair) -> Tuple[float, Dict]:
        """
        计算单个偏好对的 DPO 损失
        
        DPO Loss = -log σ(β * (log(π_θ(y_w)/π_ref(y_w)) - log(π_θ(y_l)/π_ref(y_l))))
        
        Args:
            pair: 偏好对
        
        Returns:
            (loss, debug_info)
        """
        # 计算参考模型的 log-prob
        log_prob_ref_chosen = self.compute_log_prob(
            pair.prompt, pair.chosen, is_reference=True
        )
        log_prob_ref_rejected = self.compute_log_prob(
            pair.prompt, pair.rejected, is_reference=True
        )
        
        # 计算策略模型的 log-prob
        log_prob_policy_chosen = self.compute_log_prob(
            pair.prompt, pair.chosen, is_reference=False
        )
        log_prob_policy_rejected = self.compute_log_prob(
            pair.prompt, pair.rejected, is_reference=False
        )
        
        # 计算 log-ratios
        log_ratio_chosen = log_prob_policy_chosen - log_prob_ref_chosen
        log_ratio_rejected = log_prob_policy_rejected - log_prob_ref_rejected
        
        # DPO 损失计算
        beta = self.config.beta
        logits = beta * (log_ratio_chosen - log_ratio_rejected)
        
        # 使用 sigmoid: -log σ(x) = log(1 + exp(-x))
        import math
        loss = math.log(1 + math.exp(-logits))
        
        debug_info = {
            "log_prob_ref_chosen": log_prob_ref_chosen,
            "log_prob_ref_rejected": log_prob_ref_rejected,
            "log_prob_policy_chosen": log_prob_policy_chosen,
            "log_prob_policy_rejected": log_prob_policy_rejected,
            "log_ratio_chosen": log_ratio_chosen,
            "log_ratio_rejected": log_ratio_rejected,
            "logits": logits,
            "loss": loss
        }
        
        return loss, debug_info
    
    def train_step(self, batch: List[PreferencePair]) -> Dict:
        """
        执行一个训练步骤
        
        Args:
            batch: 批次偏好对
        
        Returns:
            训练统计信息
        """
        total_loss = 0.0
        all_debug_info = []
        
        print(f"\n  处理批次 ({len(batch)} 个样本)...")
        
        for i, pair in enumerate(batch):
            loss, debug_info = self.compute_dpo_loss(pair)
            total_loss += loss
            all_debug_info.append(debug_info)
            
            # 显示前几个样本的详细信息
            if i < 2:
                print(f"\n    样本 {i+1}:")
                print(f"      Prompt: {pair.prompt[:50]}...")
                print(f"      Loss: {loss:.4f}")
                print(f"      Logits: {debug_info['logits']:.4f}")
        
        avg_loss = total_loss / len(batch) if batch else 0.0
        
        # 模拟策略更新：通过调整温度来模拟学习效果
        # 实际 DPO 会更新模型权重，这里我们通过降低温度来模拟策略改进
        improvement = max(0.01, avg_loss * 0.1)
        old_temp = self.policy_llm.temperature
        self.policy_llm.temperature = max(0.1, old_temp - improvement * 0.01)
        
        return {
            "avg_loss": avg_loss,
            "temperature": self.policy_llm.temperature,
            "batch_size": len(batch),
            "details": all_debug_info
        }
    
    def train(self, dataset: PreferenceDataset, num_epochs: int = None) -> List[Dict]:
        """
        执行完整训练
        
        Args:
            dataset: 偏好数据集
            num_epochs: 训练轮数
        
        Returns:
            训练历史
        """
        epochs = num_epochs or self.config.num_epochs
        
        print(f"\n{'='*60}")
        print("开始 DPO 训练")
        print(f"{'='*60}")
        print(f"配置:")
        print(f"  Beta: {self.config.beta}")
        print(f"  Epochs: {epochs}")
        print(f"  Batch Size: {self.config.batch_size}")
        print(f"  数据集大小: {len(dataset)}")
        
        for epoch in range(epochs):
            print(f"\n{'-'*60}")
            print(f"Epoch {epoch + 1}/{epochs}")
            print(f"{'-'*60}")
            
            # 模拟多个批次
            num_batches = max(1, len(dataset) // self.config.batch_size)
            epoch_losses = []
            
            for batch_idx in range(num_batches):
                batch = dataset.get_batch(self.config.batch_size)
                stats = self.train_step(batch)
                epoch_losses.append(stats["avg_loss"])
                
                print(f"  Batch {batch_idx + 1}/{num_batches}: Loss = {stats['avg_loss']:.4f}, "
                      f"Temp = {stats['temperature']:.3f}")
            
            avg_epoch_loss = sum(epoch_losses) / len(epoch_losses)
            
            self.training_history.append({
                "epoch": epoch + 1,
                "avg_loss": avg_epoch_loss,
                "temperature": self.policy_llm.temperature
            })
            
            print(f"\n  Epoch {epoch + 1} 完成 - 平均 Loss: {avg_epoch_loss:.4f}")
        
        print(f"\n{'='*60}")
        print("训练完成")
        print(f"{'='*60}")
        
        return self.training_history


# ============ 4. 偏好数据收集 ============

class PreferenceCollector:
    """
    偏好数据收集器
    
    用于收集人类反馈，构建偏好数据集。
    支持：
    - 人工标注
    - LLM 作为评判者
    - 自动对比生成
    """
    
    def __init__(self):
        self.llm = ChatOpenAI(
            model="deepseek-chat",
            temperature=0,
            base_url="https://api.deepseek.com/v1",
            api_key=os.getenv("DEEPSEEK_API_KEY")
        )
        
        self.comparison_prompt = ChatPromptTemplate.from_messages([
            ("system", """你是一个公正的评判者。比较两个 AI 回答，判断哪个更好。

评判标准：
1. 准确性：信息是否正确
2. 有用性：是否真正回答了问题
3. 清晰度：表达是否清晰易懂
4. 安全性：是否包含有害内容

输出格式：
- 如果 A 更好，返回: A
- 如果 B 更好，返回: B
- 如果同等好/差，返回: TIE

只返回字母，不要解释。"""),
            ("human", """问题: {prompt}

回答 A:
{response_a}

回答 B:
{response_b}

哪个更好？""")
        ])
        
        self.comparison_chain = self.comparison_prompt | self.llm | StrOutputParser()
    
    def collect_human_preference(self, prompt: str, response_a: str, 
                                  response_b: str, preferred: str,
                                  strength: float = 1.0) -> PreferencePair:
        """
        收集人类偏好
        
        Args:
            prompt: 输入提示
            response_a: 回答 A
            response_b: 回答 B
            preferred: "A" 或 "B"
            strength: 偏好强度
        
        Returns:
            PreferencePair
        """
        if preferred == "A":
            return PreferencePair(
                prompt=prompt,
                chosen=response_a,
                rejected=response_b,
                preference_strength=strength
            )
        else:
            return PreferencePair(
                prompt=prompt,
                chosen=response_b,
                rejected=response_a,
                preference_strength=strength
            )
    
    def llm_judge(self, prompt: str, response_a: str, 
                  response_b: str) -> Tuple[str, float]:
        """
        使用 LLM 作为评判者
        
        Returns:
            (preferred, confidence)
            preferred: "A", "B", 或 "TIE"
            confidence: 置信度 0-1
        """
        try:
            result = self.comparison_chain.invoke({
                "prompt": prompt,
                "response_a": response_a,
                "response_b": response_b
            })
            
            result = result.strip().upper()
            
            if "A" in result:
                return "A", 0.8
            elif "B" in result:
                return "B", 0.8
            else:
                return "TIE", 0.5
                
        except Exception as e:
            print(f"LLM 评判时出错: {e}")
            return "TIE", 0.0
    
    def generate_comparison_data(self, prompts: List[str], 
                                  model_a, model_b) -> PreferenceDataset:
        """
        自动生成对比数据
        
        使用两个不同的模型生成回答，然后用 LLM 评判。
        
        Args:
            prompts: 提示列表
            model_a: 模型 A（通常是参考模型）
            model_b: 模型 B（通常是待优化模型）
        
        Returns:
            PreferenceDataset
        """
        dataset = PreferenceDataset()
        
        print(f"\n生成对比数据 ({len(prompts)} 个提示)...")
        
        for i, prompt in enumerate(prompts):
            print(f"  处理 {i+1}/{len(prompts)}: {prompt[:40]}...")
            
            # 生成两个回答
            try:
                response_a = model_a.invoke(prompt).content
                response_b = model_b.invoke(prompt).content
                
                # LLM 评判
                preferred, confidence = self.llm_judge(prompt, response_a, response_b)
                
                if preferred != "TIE":
                    pair = self.collect_human_preference(
                        prompt, response_a, response_b, preferred, confidence
                    )
                    dataset.add_pair(pair)
                    print(f"    → 偏好: {preferred} (置信度: {confidence:.2f})")
                else:
                    print(f"    → 平局，跳过")
                    
            except Exception as e:
                print(f"    → 错误: {e}")
        
        print(f"\n生成了 {len(dataset)} 个偏好对")
        return dataset


# ============ 5. 策略评估 ============

class PolicyEvaluator:
    """
    策略评估器
    
    评估训练后的策略效果。
    """
    
    def __init__(self):
        self.llm = ChatOpenAI(
            model="deepseek-chat",
            temperature=0,
            base_url="https://api.deepseek.com/v1",
            api_key=os.getenv("DEEPSEEK_API_KEY")
        )
    
    def evaluate_win_rate(self, policy_model, reference_model, 
                          test_prompts: List[str], judge_model=None) -> float:
        """
        评估策略相对于参考模型的胜率
        
        Args:
            policy_model: 训练后的策略模型
            reference_model: 参考模型
            test_prompts: 测试提示
            judge_model: 评判模型（默认使用 self.llm）
        
        Returns:
            胜率 (0-1)
        """
        judge = judge_model or self.llm
        collector = PreferenceCollector()
        
        wins = 0
        total = 0
        
        print(f"\n评估胜率 ({len(test_prompts)} 个测试用例)...")
        
        for prompt in test_prompts:
            try:
                policy_response = policy_model.invoke(prompt).content
                ref_response = reference_model.invoke(prompt).content
                
                preferred, _ = collector.llm_judge(prompt, policy_response, ref_response)
                
                if preferred == "A":  # 假设 A 是 policy
                    wins += 1
                    result = "胜"
                elif preferred == "B":
                    result = "负"
                else:
                    result = "平"
                
                total += 1
                print(f"  {prompt[:30]}... -> {result}")
                
            except Exception as e:
                print(f"  错误: {e}")
        
        win_rate = wins / total if total > 0 else 0.0
        print(f"\n胜率: {win_rate:.2%} ({wins}/{total})")
        
        return win_rate
    
    def qualitative_evaluation(self, policy_model, test_prompts: List[str]) -> List[Dict]:
        """
        定性评估：展示模型回答
        
        Returns:
            评估结果列表
        """
        results = []
        
        print(f"\n定性评估 ({len(test_prompts)} 个示例):")
        print("="*60)
        
        for prompt in test_prompts:
            try:
                response = policy_model.invoke(prompt).content
                
                result = {
                    "prompt": prompt,
                    "response": response
                }
                results.append(result)
                
                print(f"\n提示: {prompt}")
                print(f"回答: {response[:200]}...")
                print("-"*60)
                
            except Exception as e:
                print(f"错误: {e}")
        
        return results


# ============ 6. 演示场景 ============

def demo_preference_data_collection():
    """演示偏好数据收集"""
    print("="*60)
    print("演示 1: 偏好数据收集")
    print("="*60)
    
    collector = PreferenceCollector()
    
    # 示例提示
    prompts = [
        "如何学习 Python 编程？",
        "解释什么是机器学习",
        "给出三个提高效率的建议"
    ]
    
    # 创建两个不同温度的模型来生成对比数据
    model_a = ChatOpenAI(
        model="deepseek-chat",
        temperature=0.0,
        base_url="https://api.deepseek.com/v1",
        api_key=os.getenv("DEEPSEEK_API_KEY")
    )
    
    model_b = ChatOpenAI(
        model="deepseek-chat",
        temperature=1.0,
        base_url="https://api.deepseek.com/v1",
        api_key=os.getenv("DEEPSEEK_API_KEY")
    )
    
    # 生成对比数据
    dataset = collector.generate_comparison_data(prompts, model_a, model_b)
    
    # 显示收集的数据
    print("\n收集的偏好数据:")
    for pair in dataset.get_all():
        print(f"\n  Prompt: {pair.prompt}")
        print(f"  Chosen: {pair.chosen[:60]}...")
        print(f"  Rejected: {pair.rejected[:60]}...")
    
    return dataset


def demo_dpo_training():
    """演示 DPO 训练"""
    print("\n" + "="*60)
    print("演示 2: DPO 训练过程")
    print("="*60)
    
    # 创建合成偏好数据（用于演示）
    dataset = PreferenceDataset()
    
    # 添加一些合成的偏好对
    preference_pairs = [
        PreferencePair(
            prompt="如何保持健康的生活方式？",
            chosen="保持健康的生活方式包括：1) 均衡饮食，多吃蔬菜水果；2) 规律运动，每周至少150分钟；3) 充足睡眠，7-8小时；4) 管理压力，通过冥想或瑜伽；5) 定期体检。",
            rejected="多吃点好的，多运动，睡够觉就行。",
            preference_strength=0.9
        ),
        PreferencePair(
            prompt="解释什么是人工智能",
            chosen="人工智能（AI）是计算机科学的一个分支，致力于创建能够执行通常需要人类智能的任务的系统。这包括学习、推理、问题解决、感知和语言理解。AI 分为弱 AI（专注于特定任务）和强 AI（具有通用智能）。",
            rejected="AI 就是让电脑变聪明的东西。",
            preference_strength=0.85
        ),
        PreferencePair(
            prompt="如何学好一门外语？",
            chosen="学好外语的有效方法：1) 沉浸式学习，尽可能多地接触目标语言；2) 每日练习，保持学习的连续性；3) 多种方式结合：听、说、读、写；4) 不怕犯错，积极使用；5) 了解文化背景，加深理解。",
            rejected="背单词，看语法书，多听多说。",
            preference_strength=0.8
        ),
        PreferencePair(
            prompt="什么是气候变化？",
            chosen="气候变化是指地球气候系统长期统计特征的变化，主要由人类活动（如燃烧化石燃料、 deforestation）导致的温室气体浓度增加引起。表现为全球变暖、极端天气增加、海平面上升等，对生态系统和人类社会构成严重威胁。",
            rejected="就是天气变热了。",
            preference_strength=0.9
        ),
    ]
    
    for pair in preference_pairs:
        dataset.add_pair(pair)
    
    print(f"\n使用 {len(dataset)} 个偏好对进行训练")
    
    # 配置和训练
    config = DPOConfig(
        beta=0.1,
        num_epochs=2,
        batch_size=2
    )
    
    trainer = DPOTrainer(config)
    history = trainer.train(dataset)
    
    # 显示训练历史
    print("\n训练历史:")
    for record in history:
        print(f"  Epoch {record['epoch']}: Loss = {record['avg_loss']:.4f}, "
              f"Temperature = {record['temperature']:.3f}")
    
    return trainer


def demo_policy_evaluation():
    """演示策略评估"""
    print("\n" + "="*60)
    print("演示 3: 策略评估")
    print("="*60)
    
    evaluator = PolicyEvaluator()
    
    # 创建策略模型和参考模型
    policy_model = ChatOpenAI(
        model="deepseek-chat",
        temperature=0.3,  # 较低温度，更确定性
        base_url="https://api.deepseek.com/v1",
        api_key=os.getenv("DEEPSEEK_API_KEY")
    )
    
    reference_model = ChatOpenAI(
        model="deepseek-chat",
        temperature=0.7,  # 较高温度，更多样
        base_url="https://api.deepseek.com/v1",
        api_key=os.getenv("DEEPSEEK_API_KEY")
    )
    
    # 测试提示
    test_prompts = [
        "如何培养良好的阅读习惯？",
        "什么是可持续发展？",
        "给出时间管理的建议"
    ]
    
    # 定性评估
    results = evaluator.qualitative_evaluation(policy_model, test_prompts)
    
    print("\n评估完成")


def demo_dpo_vs_rlhf():
    """对比 DPO 和传统 RLHF"""
    print("\n" + "="*60)
    print("演示 4: DPO vs RLHF 对比")
    print("="*60)
    
    comparison = """
【DPO (Direct Preference Optimization)】

优势：
1. 简单直接：无需训练奖励模型
2. 计算高效：只需要策略模型，不需要额外的价值网络
3. 训练稳定：避免了 RL 的不稳定性（如奖励黑客）
4. 理论保证：有坚实的数学基础
5. 易于实现：可以看作是一种特殊的分类损失

劣势：
1. 对数据质量要求高：需要高质量的偏好对
2. 泛化能力：在某些复杂任务上可能不如 RLHF
3. 超参数敏感：β 参数需要仔细调优

【传统 RLHF (PPO)】

优势：
1. 灵活性高：可以使用复杂的奖励函数
2. 在线学习：可以持续从环境中学习
3. 成熟稳定：有大量实践经验和优化技巧

劣势：
1. 复杂度高：需要训练奖励模型 + 策略模型 + 价值模型
2. 计算成本高：需要大量计算资源
3. 训练不稳定：容易出现模式崩溃、奖励黑客等问题
4. 难以调试：多个组件相互影响

【适用场景】

DPO 更适合：
- 有高质量离线偏好数据的场景
- 计算资源有限的场景
- 需要快速迭代的场景
- 对训练稳定性要求高的场景

RLHF 更适合：
- 需要在线学习的场景
- 奖励函数复杂的场景
- 有大量计算资源的场景
- 需要精细控制优化过程的场景
"""
    
    print(comparison)


def demo_practical_tips():
    """DPO 实践建议"""
    print("\n" + "="*60)
    print("演示 5: DPO 实践建议")
    print("="*60)
    
    tips = """
【数据准备】

1. 偏好对质量
   - 确保 chosen 和 rejected 有明确的质量差异
   - 避免模糊或主观的偏好
   - 偏好的边界要清晰

2. 数据多样性
   - 覆盖不同的任务类型
   - 包含不同难度的样本
   - 避免数据偏见

3. 数据量
   - 通常需要数千到数万个偏好对
   - 质量比数量更重要
   - 可以通过 LLM 辅助生成初始数据

【超参数调优】

1. Beta (β)
   - 默认值 0.1 是一个好的起点
   - 较大的 β：策略更接近参考模型，更保守
   - 较小的 β：策略更激进，可能偏离参考模型更远
   - 建议范围：0.01 - 0.5

2. 学习率
   - 通常比 SFT 低一个数量级
   - 建议范围：1e-6 到 1e-5
   - 使用学习率预热和衰减

3. 批次大小
   - 越大越好，但受限于显存
   - 建议至少 32 或 64

【训练技巧】

1. 参考模型冻结
   - 参考模型应该保持固定
   - 不要更新参考模型的权重

2. 梯度裁剪
   - 使用梯度裁剪防止梯度爆炸
   - 建议 max_grad_norm = 1.0

3. 早停
   - 监控验证集上的性能
   - 避免过拟合

4. 混合数据
   - 可以混合 SFT 数据和 DPO 数据
   - 防止模型遗忘预训练知识

【评估指标】

1. 胜率 (Win Rate)
   - 与参考模型或基线模型对比
   - 使用 GPT-4 或其他强模型作为评判

2. 损失曲线
   - 监控 DPO 损失的变化
   - 应该逐渐下降并收敛

3. 人工评估
   - 定期进行人工质量评估
   - 关注安全性、有用性、诚实性

【常见问题】

1. 损失不下降
   - 检查数据质量
   - 降低学习率
   - 增大 β 值

2. 模型退化
   - 可能是 β 太小
   - 增加 SFT 数据混合
   - 使用更保守的更新策略

3. 过拟合
   - 增加正则化
   - 早停
   - 增加数据多样性
"""
    
    print(tips)


# ============ 7. 主函数 ============

def main():
    """主函数：运行所有演示"""
    print("="*60)
    print("Chapter 10: Learning and Adaptation Pattern")
    print("Direct Preference Optimization (DPO)")
    print("="*60)
    
    # 运行各个演示
    dataset = demo_preference_data_collection()
    trainer = demo_dpo_training()
    demo_policy_evaluation()
    demo_dpo_vs_rlhf()
    demo_practical_tips()
    
    # 总结
    print("\n" + "="*60)
    print("DPO 总结")
    print("="*60)
    print("""
【核心概念】

1. **直接偏好优化 (DPO)**
   - 直接从偏好数据学习，无需奖励模型
   - 将 RL 问题转化为简单的分类问题
   - 数学上等价于带 Bradley-Terry 模型的 RLHF

2. **关键公式**
   L_DPO = -log σ(β * (log(π_θ(y_w)/π_ref(y_w)) - log(π_θ(y_l)/π_ref(y_l))))
   
   - π_θ: 策略模型（待优化）
   - π_ref: 参考模型（固定）
   - y_w: 偏好的回答
   - y_l: 不喜欢的回答
   - β: 温度系数

3. **训练流程**
   - 准备偏好对数据 (prompt, chosen, rejected)
   - 初始化参考模型（通常是 SFT 模型）
   - 计算 DPO 损失
   - 更新策略模型
   - 评估和迭代

【优势】

- 简单：无需训练奖励模型
- 高效：计算成本低
- 稳定：避免了 RL 的不稳定性
- 有效：在实践中表现优异

【最佳实践】

1. 数据质量 > 数据数量
2. 仔细调优 β 参数
3. 使用适当的学习率
4. 混合 SFT 数据防止遗忘
5. 定期评估和人工检查

【与其他模式结合】

- + Reflection: 基于反馈进行自我改进
- + Planning: 学习更好的规划策略
- + Multi-Agent: 多智能体之间的偏好学习
- + Memory: 存储和复用偏好经验
""")


if __name__ == "__main__":
    main()
