"""
Chapter 16: Resource-Aware Optimization (资源感知优化模式)

资源感知优化是 Agentic Design Patterns 中的高级效率模式。
它让智能体在运行时动态监控和管理计算、时间、财务资源，
根据任务复杂度和资源约束选择最优的执行策略。

核心思想：
- 不是所有任务都需要最强大的模型
- 简单查询用低成本模型，复杂查询用高性能模型
- 在预算、延迟、质量之间做动态权衡
- 当首选资源不可用时，自动降级保证服务连续性

典型应用场景：
1. 成本优化的 LLM 使用：根据查询复杂度选择不同价位的模型
2. 延迟敏感操作：实时系统中选择更快但可能较浅的推理路径
3. 服务可靠性回退：主模型过载时自动切换到备用模型
4. 自适应任务分配：多智能体系统中根据负载动态分配任务

本示例实现：
1. 查询复杂度分类器
2. 基于预算和时间约束的资源路由器
3. Tavily 搜索工具（用于需要实时信息的查询）
4. Critique Agent（评估路由决策质量）
5. Fallback 机制（模型失败时的降级策略）
"""

import os
import time
import json
from typing import Dict, List, Optional, Tuple
from dataclasses import dataclass, field
from enum import Enum

from langchain_openai import ChatOpenAI
from langchain_core.prompts import ChatPromptTemplate
from langchain_core.output_parsers import StrOutputParser
from langchain_community.tools import TavilySearchResults
from dotenv import load_dotenv

load_dotenv()


# ============ 1. 数据模型与枚举 ============

class QueryComplexity(Enum):
    """查询复杂度等级"""
    SIMPLE = "simple"           # 简单事实查询
    REASONING = "reasoning"     # 需要推理的多步问题
    SEARCH_NEEDED = "search"    # 需要实时信息


class ModelTier(Enum):
    """模型层级（对应不同成本）"""
    ECONOMY = "economy"         # 低成本快速响应
    STANDARD = "standard"       # 标准均衡
    PREMIUM = "premium"         # 高成本高性能


@dataclass
class ResourceBudget:
    """资源预算"""
    max_cost: float = 10.0          # 最大成本预算（单位：元）
    max_latency_ms: int = 30000     # 最大延迟（毫秒）
    current_cost: float = field(default=0.0)
    current_latency_ms: int = field(default=0)

    def can_afford(self, estimated_cost: float, estimated_latency_ms: int) -> bool:
        """检查预算是否足够"""
        return (self.current_cost + estimated_cost <= self.max_cost and
                self.current_latency_ms + estimated_latency_ms <= self.max_latency_ms)

    def consume(self, cost: float, latency_ms: int):
        """消耗资源"""
        self.current_cost += cost
        self.current_latency_ms += latency_ms

    def remaining_ratio(self) -> float:
        """剩余预算比例"""
        cost_ratio = 1.0 - (self.current_cost / self.max_cost) if self.max_cost > 0 else 0
        latency_ratio = 1.0 - (self.current_latency_ms / self.max_latency_ms) if self.max_latency_ms > 0 else 0
        return min(cost_ratio, latency_ratio)

    def __repr__(self):
        return (f"Budget(cost={self.current_cost:.2f}/{self.max_cost:.2f}, "
                f"latency={self.current_latency_ms}ms/{self.max_latency_ms}ms, "
                f"remaining={self.remaining_ratio():.1%})")


@dataclass
class ModelConfig:
    """模型配置"""
    name: str
    tier: ModelTier
    estimated_cost_per_1k_tokens: float  # 每1K token估计成本
    estimated_latency_ms_per_1k_tokens: int
    temperature: float
    description: str


# ============ 2. 模型配置表 ============

# 使用 DeepSeek API，通过不同参数模拟不同成本层级
MODEL_CONFIGS = {
    "deepseek-chat-fast": ModelConfig(
        name="deepseek-chat",
        tier=ModelTier.ECONOMY,
        estimated_cost_per_1k_tokens=0.002,
        estimated_latency_ms_per_1k_tokens=500,
        temperature=0.7,
        description="低成本快速模型，适合简单问答"
    ),
    "deepseek-chat": ModelConfig(
        name="deepseek-chat",
        tier=ModelTier.STANDARD,
        estimated_cost_per_1k_tokens=0.005,
        estimated_latency_ms_per_1k_tokens=1000,
        temperature=0.3,
        description="标准模型，均衡成本与质量"
    ),
    "deepseek-reasoner": ModelConfig(
        name="deepseek-reasoner",
        tier=ModelTier.PREMIUM,
        estimated_cost_per_1k_tokens=0.016,
        estimated_latency_ms_per_1k_tokens=3000,
        temperature=0.3,
        description="高性能推理模型，适合复杂问题"
    ),
}


def get_llm(config: ModelConfig) -> ChatOpenAI:
    """根据配置创建 LLM 实例"""
    return ChatOpenAI(
        model=config.name,
        temperature=config.temperature,
        base_url="https://api.deepseek.com/v1",
        api_key=os.getenv("DEEPSEEK_API_KEY")
    )


# ============ 3. 查询分类器 ============

class QueryClassifier:
    """
    查询复杂度分类器

    将用户查询分为三个等级：
    - simple: 直接事实问答，无需推理
    - reasoning: 逻辑、数学、多步推理
    - search: 需要实时信息或当前事件
    """

    CLASSIFICATION_PROMPT = """你是一个查询分类器。分析用户查询并返回以下三个类别之一：

- simple: 简单事实查询，可以直接回答，无需推理或实时信息
  例如："法国的首都是哪里？"、"2+2等于几？"、"Python是什么？"

- reasoning: 需要逻辑推理、数学计算、多步分析的问题
  例如："如果一个水池有两个进水管..."、"分析量子计算对密码学的影响"、"比较React和Vue的优劣"

- search: 需要实时信息、当前事件、最新数据的问题
  例如："今天的天气如何？"、"最新的AI新闻是什么？"、"现在黄金价格多少？"

规则：
1. 如果查询涉及"最新"、"现在"、"今天"、"最近"等时间词，优先归为 search
2. 如果查询需要多步逻辑推导，归为 reasoning
3. 简单的事实性问题归为 simple

只返回 JSON 格式：{{"classification": "simple|reasoning|search"}}
不要输出其他任何内容。"""

    def __init__(self):
        # 分类器本身用经济型模型即可
        self.llm = get_llm(MODEL_CONFIGS["deepseek-chat-fast"])
        self.prompt = ChatPromptTemplate.from_messages([
            ("system", self.CLASSIFICATION_PROMPT),
            ("human", "查询: {query}")
        ])
        self.chain = self.prompt | self.llm | StrOutputParser()

    def classify(self, query: str) -> QueryComplexity:
        """分类查询"""
        try:
            result = self.chain.invoke({"query": query})
            # 尝试解析 JSON
            if "simple" in result.lower():
                return QueryComplexity.SIMPLE
            elif "reasoning" in result.lower():
                return QueryComplexity.REASONING
            elif "search" in result.lower():
                return QueryComplexity.SEARCH_NEEDED
            else:
                # 回退：基于启发式规则
                return self._heuristic_classify(query)
        except Exception as e:
            print(f"  ⚠️ 分类失败 ({e})，使用启发式分类")
            return self._heuristic_classify(query)

    def _heuristic_classify(self, query: str) -> QueryComplexity:
        """启发式分类（不调用LLM）"""
        query_lower = query.lower()
        # 搜索关键词
        search_keywords = ["最新", "现在", "今天", "最近", "当前", "新闻", "价格", "天气"]
        if any(kw in query_lower for kw in search_keywords):
            return QueryComplexity.SEARCH_NEEDED
        # 推理关键词
        reasoning_keywords = ["为什么", "如何", "分析", "比较", "如果", "假设", "计算", "推理"]
        if any(kw in query_lower for kw in reasoning_keywords) or len(query) > 50:
            return QueryComplexity.REASONING
        return QueryComplexity.SIMPLE


# ============ 4. 资源路由器 ============

class ResourceRouter:
    """
    资源感知路由器

    根据查询复杂度和当前资源预算，选择最优的模型配置。
    核心决策逻辑：
    - 预算充足时，按复杂度匹配最佳模型
    - 预算紧张时，自动降级以节省资源
    - 始终保证服务可用性
    """

    # 复杂度到推荐模型的映射
    COMPLEXITY_MODEL_MAP = {
        QueryComplexity.SIMPLE: "deepseek-chat-fast",
        QueryComplexity.REASONING: "deepseek-reasoner",
        QueryComplexity.SEARCH_NEEDED: "deepseek-chat",
    }

    # 降级映射
    FALLBACK_MAP = {
        "deepseek-reasoner": "deepseek-chat",
        "deepseek-chat": "deepseek-chat-fast",
        "deepseek-chat-fast": None,  # 最低层级，无降级
    }

    def __init__(self, budget: ResourceBudget):
        self.budget = budget
        self.routing_history: List[Dict] = []

    def select_model(self, query: str, complexity: QueryComplexity) -> Tuple[str, str]:
        """
        选择模型，返回 (model_key, reason)
        """
        recommended = self.COMPLEXITY_MODEL_MAP[complexity]
        config = MODEL_CONFIGS[recommended]

        # 估算资源消耗（假设平均输出 500 tokens）
        estimated_cost = config.estimated_cost_per_1k_tokens * 0.5
        estimated_latency = config.estimated_latency_ms_per_1k_tokens * 0.5

        # 检查预算
        if self.budget.can_afford(estimated_cost, estimated_latency):
            reason = (f"查询复杂度为 {complexity.value}，"
                      f"预算充足 ({self.budget.remaining_ratio():.0%} 剩余)，"
                      f"使用推荐模型 {recommended}")
            self.routing_history.append({
                "query": query,
                "complexity": complexity.value,
                "selected": recommended,
                "reason": reason,
                "downgraded": False
            })
            return recommended, reason

        # 预算不足，尝试降级
        current = recommended
        while current:
            fallback = self.FALLBACK_MAP.get(current)
            if not fallback:
                break
            config = MODEL_CONFIGS[fallback]
            estimated_cost = config.estimated_cost_per_1k_tokens * 0.5
            estimated_latency = config.estimated_latency_ms_per_1k_tokens * 0.5
            if self.budget.can_afford(estimated_cost, estimated_latency):
                reason = (f"查询复杂度为 {complexity.value}，"
                          f"但预算紧张 ({self.budget.remaining_ratio():.0%} 剩余)，"
                          f"从 {current} 降级到 {fallback}")
                self.routing_history.append({
                    "query": query,
                    "complexity": complexity.value,
                    "selected": fallback,
                    "reason": reason,
                    "downgraded": True
                })
                return fallback, reason
            current = fallback

        # 预算完全耗尽，强制使用最低成本模型
        reason = (f"预算几乎耗尽 ({self.budget.remaining_ratio():.0%} 剩余)，"
                  f"强制使用最低成本模型 deepseek-chat-fast")
        self.routing_history.append({
            "query": query,
            "complexity": complexity.value,
            "selected": "deepseek-chat-fast",
            "reason": reason,
            "downgraded": True
        })
        return "deepseek-chat-fast", reason


# ============ 5. 搜索工具 ============

class SearchTool:
    """
    Tavily 搜索工具
    用于获取实时信息
    """

    def __init__(self):
        self.search_engine = TavilySearchResults(
            max_results=3,
            api_key=os.getenv("TAVILY_API_KEY")
        )

    def search(self, query: str) -> str:
        """执行搜索并返回格式化的结果"""
        try:
            results = self.search_engine.invoke(query)
            if not results:
                return "未找到相关搜索结果。"

            formatted = []
            for i, result in enumerate(results, 1):
                content = result.get("content", "")
                url = result.get("url", "")
                formatted.append(f"[{i}] {content[:200]}...\n    来源: {url}")

            return "\n\n".join(formatted)
        except Exception as e:
            return f"搜索出错: {str(e)}"


# ============ 6. Critique Agent ============

class CritiqueAgent:
    """
    批评/评估 Agent

    评估资源路由决策的质量：
    - 是否将简单查询路由到了昂贵模型（浪费）
    - 是否将复杂查询路由到了廉价模型（质量不足）
    - 提供优化建议
    """

    SYSTEM_PROMPT = """你是资源优化评估专家。你的职责是评估路由决策的质量。

评估维度：
1. 路由合理性：查询复杂度与所选模型层级是否匹配
2. 预算效率：是否在保证质量的前提下最小化成本
3. 降级必要性：降级是否合理，还是过于保守

反馈格式：
- 评分: 1-10（10为最优）
- 评价: 简要说明路由决策的优缺点
- 建议: 如何改进资源分配

请用中文回答，简洁明了。"""

    def __init__(self):
        self.llm = get_llm(MODEL_CONFIGS["deepseek-chat"])
        self.prompt = ChatPromptTemplate.from_messages([
            ("system", self.SYSTEM_PROMPT),
            ("human", """请评估以下路由决策：

用户查询: {query}
查询复杂度: {complexity}
选择模型: {model}
降级状态: {downgraded}
预算剩余: {budget_remaining}

路由理由: {reason}

请给出评分和评价。""")
        ])
        self.chain = self.prompt | self.llm | StrOutputParser()

    def evaluate(self, routing_record: Dict, budget: ResourceBudget) -> str:
        """评估路由决策"""
        try:
            return self.chain.invoke({
                "query": routing_record["query"],
                "complexity": routing_record["complexity"],
                "model": routing_record["selected"],
                "downgraded": "是" if routing_record["downgraded"] else "否",
                "budget_remaining": f"{budget.remaining_ratio():.1%}",
                "reason": routing_record["reason"]
            })
        except Exception as e:
            return f"评估失败: {str(e)}"


# ============ 7. 主系统：资源感知问答引擎 ============

class ResourceAwareQASystem:
    """
    资源感知问答系统

    整合所有组件，实现完整的资源感知优化流程：
    1. 分类查询复杂度
    2. 根据预算选择模型
    3. 如需搜索则获取实时信息
    4. 生成响应
    5. 更新预算
    6. Critique Agent 评估
    """

    def __init__(self, budget: Optional[ResourceBudget] = None):
        self.budget = budget or ResourceBudget()
        self.classifier = QueryClassifier()
        self.router = ResourceRouter(self.budget)
        self.search_tool = SearchTool()
        self.critique = CritiqueAgent()
        self.query_count = 0

    def answer(self, query: str, verbose: bool = True) -> Dict:
        """
        回答用户查询

        Returns:
            Dict 包含 classification, model, response, cost, latency, critique
        """
        self.query_count += 1
        start_time = time.time()

        if verbose:
            print(f"\n{'─' * 70}")
            print(f"查询 #{self.query_count}: {query}")
            print('─' * 70)

        # Step 1: 分类查询
        complexity = self.classifier.classify(query)
        if verbose:
            print(f"📊 复杂度分类: {complexity.value}")

        # Step 2: 选择模型
        model_key, reason = self.router.select_model(query, complexity)
        config = MODEL_CONFIGS[model_key]
        if verbose:
            print(f"🎯 路由决策: {config.description}")
            print(f"   理由: {reason}")

        # Step 3: 获取实时信息（如果需要）
        search_context = ""
        if complexity == QueryComplexity.SEARCH_NEEDED:
            if verbose:
                print("🔍 执行搜索获取实时信息...")
            search_context = self.search_tool.search(query)
            if verbose:
                print(f"   搜索结果: {search_context[:200]}...")

        # Step 4: 生成响应
        if verbose:
            print(f"🧠 使用模型生成响应...")

        try:
            llm = get_llm(config)
            response_text = self._generate_response(llm, query, complexity, search_context)
            model_used = config.name
        except Exception as e:
            # Fallback: 降级到标准模型
            if verbose:
                print(f"   ⚠️ 模型调用失败: {e}，触发 Fallback...")
            fallback_key = self.router.FALLBACK_MAP.get(model_key, "deepseek-chat-fast")
            fallback_config = MODEL_CONFIGS[fallback_key]
            llm = get_llm(fallback_config)
            response_text = self._generate_response(llm, query, complexity, search_context)
            model_used = f"{config.name} (fallback to {fallback_config.name})"

        # Step 5: 计算资源消耗
        elapsed_ms = int((time.time() - start_time) * 1000)
        # 估算 token 消耗（输入 + 输出）
        estimated_tokens = len(query) + len(response_text)
        estimated_cost = config.estimated_cost_per_1k_tokens * (estimated_tokens / 1000)
        self.budget.consume(estimated_cost, elapsed_ms)

        if verbose:
            print(f"⏱️  耗时: {elapsed_ms}ms | 💰 估算成本: ¥{estimated_cost:.4f}")
            print(f"📦 预算状态: {self.budget}")

        # Step 6: Critique 评估
        routing_record = self.router.routing_history[-1]
        critique_feedback = self.critique.evaluate(routing_record, self.budget)

        if verbose:
            print(f"\n🔍 Critique 评估:")
            print(f"   {critique_feedback[:300]}...")

        return {
            "query": query,
            "classification": complexity.value,
            "model_used": model_used,
            "model_tier": config.tier.value,
            "response": response_text,
            "latency_ms": elapsed_ms,
            "estimated_cost": estimated_cost,
            "budget_remaining": self.budget.remaining_ratio(),
            "critique": critique_feedback,
            "downgraded": routing_record["downgraded"]
        }

    def _generate_response(
        self,
        llm: ChatOpenAI,
        query: str,
        complexity: QueryComplexity,
        search_context: str
    ) -> str:
        """生成响应"""
        if complexity == QueryComplexity.SEARCH_NEEDED and search_context:
            prompt = ChatPromptTemplate.from_messages([
                ("system", "你是一个智能助手。请根据提供的搜索结果回答用户问题。"),
                ("human", """搜索结果:
{search_context}

用户问题: {query}

请基于搜索结果给出准确、简洁的回答。""")
            ])
            chain = prompt | llm | StrOutputParser()
            return chain.invoke({"query": query, "search_context": search_context})
        else:
            prompt = ChatPromptTemplate.from_messages([
                ("system", "你是一个有帮助的助手，请准确回答用户问题。"),
                ("human", "{query}")
            ])
            chain = prompt | llm | StrOutputParser()
            return chain.invoke({"query": query})

    def print_summary(self):
        """打印系统运行摘要"""
        print(f"\n{'=' * 70}")
        print("资源感知问答系统 - 运行摘要")
        print('=' * 70)
        print(f"总查询数: {self.query_count}")
        print(f"总估算成本: ¥{self.budget.current_cost:.4f} / ¥{self.budget.max_cost:.2f}")
        print(f"总耗时: {self.budget.current_latency_ms}ms / {self.budget.max_latency_ms}ms")
        print(f"预算剩余: {self.budget.remaining_ratio():.1%}")
        print(f"\n路由历史:")
        for i, record in enumerate(self.router.routing_history, 1):
            downgrade_mark = " ⬇️降级" if record["downgraded"] else ""
            print(f"  {i}. [{record['complexity']}] -> {record['selected']}{downgrade_mark}")


# ============ 8. 对比：无资源感知的简单系统 ============

class SimpleQASystem:
    """
    简单问答系统（无资源感知优化）
    所有查询使用同一个模型，用于对比展示资源感知优化的优势
    """

    def __init__(self):
        self.llm = get_llm(MODEL_CONFIGS["deepseek-reasoner"])
        self.prompt = ChatPromptTemplate.from_messages([
            ("system", "你是一个有帮助的助手。"),
            ("human", "{query}")
        ])
        self.chain = self.prompt | self.llm | StrOutputParser()
        self.total_cost = 0.0
        self.query_count = 0

    def answer(self, query: str) -> str:
        """回答查询（始终使用最高成本模型）"""
        self.query_count += 1
        start = time.time()
        response = self.chain.invoke({"query": query})
        elapsed_ms = int((time.time() - start) * 1000)

        # 估算成本（使用最高成本模型）
        estimated_tokens = len(query) + len(response)
        cost = MODEL_CONFIGS["deepseek-reasoner"].estimated_cost_per_1k_tokens * (estimated_tokens / 1000)
        self.total_cost += cost

        return response

    def print_summary(self):
        print(f"\n总查询数: {self.query_count}")
        print(f"总估算成本: ¥{self.total_cost:.4f}（所有查询使用 Premium 模型）")


# ============ 9. 演示 ============

def demo_resource_aware_system():
    """演示资源感知优化系统"""
    print("=" * 70)
    print("Chapter 16: Resource-Aware Optimization (资源感知优化)")
    print("=" * 70)
    print("""
本示例展示 Resource-Aware Optimization 模式的核心能力：
1. 查询分类：自动判断查询复杂度
2. 智能路由：根据预算选择最合适的模型
3. 动态降级：预算紧张时自动使用低成本模型
4. 实时搜索：需要时获取最新信息
5. Critique 评估：持续优化路由决策
    """)

    # 设置预算（模拟有限资源场景）
    budget = ResourceBudget(max_cost=0.5, max_latency_ms=60000)
    system = ResourceAwareQASystem(budget)

    # 测试查询集（涵盖三种复杂度）
    test_queries = [
        # 简单查询
        "法国的首都是哪里？",
        "2 + 2 等于几？",
        # 推理查询
        "请分析一下深度学习在医疗影像诊断中的优势与挑战",
        "如果一个水池有两个进水管，A管单独注满需要6小时，B管单独注满需要4小时，同时打开两管需要多久注满？",
        # 需要搜索的查询
        "最近人工智能领域有什么重要突破？",
    ]

    results = []
    for query in test_queries:
        result = system.answer(query, verbose=True)
        results.append(result)
        print(f"\n💬 响应: {result['response'][:200]}...")

    system.print_summary()

    return results


def demo_comparison():
    """对比：资源感知 vs 无资源感知"""
    print("\n" + "=" * 70)
    print("对比演示：资源感知 vs 无资源感知")
    print("=" * 70)

    queries = [
        "法国的首都是哪里？",
        "2 + 2 等于几？",
        "请解释量子计算的基本原理",
    ]

    # 资源感知系统（小预算）
    print("\n【方案 A】资源感知系统（预算 ¥0.3）")
    budget = ResourceBudget(max_cost=0.3, max_latency_ms=60000)
    aware_system = ResourceAwareQASystem(budget)
    for q in queries:
        aware_system.answer(q, verbose=False)
    aware_system.print_summary()

    # 无资源感知系统（始终用最高成本模型）
    print("\n【方案 B】无资源感知系统（始终使用 Premium 模型）")
    simple_system = SimpleQASystem()
    for q in queries:
        simple_system.answer(q)
    simple_system.print_summary()


def demo_budget_exhaustion():
    """演示预算耗尽时的降级行为"""
    print("\n" + "=" * 70)
    print("演示：预算耗尽时的自动降级")
    print("=" * 70)
    print("设置极低的预算，观察系统如何自动降级...")

    budget = ResourceBudget(max_cost=0.05, max_latency_ms=60000)
    system = ResourceAwareQASystem(budget)

    # 连续发送多个查询，迫使预算耗尽
    queries = [
        "分析机器学习的未来发展趋势",  # reasoning -> premium
        "解释神经网络反向传播算法",    # reasoning -> premium
        "法国的首都是哪里？",          # simple -> economy
    ]

    for q in queries:
        result = system.answer(q, verbose=True)
        print(f"\n💬 响应: {result['response'][:150]}...")

    system.print_summary()


def main():
    # 主演示
    demo_resource_aware_system()

    # 对比演示
    demo_comparison()

    # 预算耗尽演示
    demo_budget_exhaustion()

    # 总结
    print("\n" + "=" * 70)
    print("Resource-Aware Optimization 优势总结")
    print("=" * 70)
    print("""
【核心优势】

1. **成本优化**
   - 简单查询使用低成本模型，节省 60-80% 费用
   - 仅在必要时使用高性能模型
   - 通过 Critique Agent 持续优化路由策略

2. **延迟优化**
   - 简单问题快速响应（<1秒）
   - 复杂问题投入更多时间获取高质量答案
   - 预算紧张时优先保证响应速度

3. **服务可靠性**
   - Fallback 机制确保模型失败时自动降级
   - 预算耗尽时仍可提供服务（质量降级）
   -  graceful degradation（优雅降级）

4. **可观测性**
   - 实时追踪成本和延迟
   - 路由决策透明可解释
   - Critique Agent 提供持续反馈

【设计要点】

1. **查询分类器**：准确判断复杂度是优化基础
2. **成本模型**：为每个模型配置合理的成本估算
3. **预算管理**：设置合理的预算上限和监控机制
4. **降级策略**：定义清晰的降级路径，避免过度降级
5. **反馈闭环**：通过 Critique Agent 持续改进路由决策

【适用场景】

- 高流量客服系统：大量简单查询需要低成本处理
- 多层级模型环境：同时拥有经济型和高性能模型
- 预算受限项目：需要在有限资源下最大化服务质量
- 实时系统：需要在延迟和质量之间做权衡

【与其他模式结合】

- + Routing: 资源感知路由是智能路由的扩展
- + Reflection: Critique Agent 提供自我反思能力
- + Multi-Agent: 不同 Agent 使用不同成本配置
- + Tool Use: 搜索等工具也是资源消耗的一部分
""")


if __name__ == "__main__":
    main()
