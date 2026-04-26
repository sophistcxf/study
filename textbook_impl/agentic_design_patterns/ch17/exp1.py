"""
Chapter 17: Reasoning Techniques (推理技术)

推理技术是 Agentic Design Patterns 中提升智能体问题解决能力的核心方法。
它让 LLM 不仅给出答案，而是展示思考过程、探索多条路径、自我修正错误。

核心思想：
- 复杂问题需要复杂推理，而非直觉回答
- 显式思考过程比隐式思考更可靠、更可调试
- 结合外部工具（代码执行）可以弥补纯文本推理的局限

书中介绍的主要技术：
1. Chain-of-Thought (CoT): 逐步思考，将大问题分解为小步骤
2. Tree-of-Thought (ToT): 探索多条推理路径，选择最优解
3. Self-Correction: 生成→评估→修正的迭代循环
4. Program-aided Reasoning: 用代码执行做精确计算
5. ReAct: 推理与行动交替进行，动态适应

本示例实现：
1. Zero-Shot vs CoT 对比演示
2. Self-Correction 迭代改进
3. Program-aided Reasoning（代码辅助推理）
4. ReAct 风格多步推理
"""

import os
import re
import json
import time
import subprocess
from typing import List, Dict, Optional, Tuple
from dataclasses import dataclass
from enum import Enum

from langchain_openai import ChatOpenAI
from langchain_core.prompts import ChatPromptTemplate
from langchain_core.output_parsers import StrOutputParser
from dotenv import load_dotenv

load_dotenv()


# ============ 1. LLM 配置 ============

# 标准模型（用于大多数任务）
llm_standard = ChatOpenAI(
    model="deepseek-chat",
    temperature=0.3,
    base_url="https://api.deepseek.com/v1",
    api_key=os.getenv("DEEPSEEK_API_KEY")
)

# 推理模型（用于复杂推理任务）
llm_reasoner = ChatOpenAI(
    model="deepseek-reasoner",
    temperature=0.3,
    base_url="https://api.deepseek.com/v1",
    api_key=os.getenv("DEEPSEEK_API_KEY")
)


# ============ 2. Demo 1: Zero-Shot vs CoT 对比 ============

def demo_zero_shot_vs_cot():
    """
    对比 Zero-Shot（直接回答）和 Chain-of-Thought（逐步思考）的效果差异
    """
    print("=" * 70)
    print("Demo 1: Zero-Shot vs Chain-of-Thought (CoT)")
    print("=" * 70)

    # 一个需要多步推理的问题
    problem = """
一个农场有鸡和兔子，共有35个头，94只脚。
请问鸡和兔子各有多少只？
"""

    print(f"\n问题: {problem.strip()}")

    # --- Zero-Shot: 直接要求答案 ---
    print("\n" + "─" * 70)
    print("【方式 A】Zero-Shot: 直接回答")
    print("─" * 70)

    zero_shot_prompt = ChatPromptTemplate.from_messages([
        ("system", "你是一个数学助手，直接给出答案。"),
        ("human", "{problem}")
    ])
    zero_shot_chain = zero_shot_prompt | llm_standard | StrOutputParser()
    zero_shot_answer = zero_shot_chain.invoke({"problem": problem})
    print(f"\n回答:\n{zero_shot_answer}")

    # --- CoT: 要求逐步思考 ---
    print("\n" + "─" * 70)
    print("【方式 B】Chain-of-Thought: 逐步思考")
    print("─" * 70)

    cot_prompt = ChatPromptTemplate.from_messages([
        ("system", """你是一个数学助手。请按照以下步骤解决问题：
1. 理解问题：明确已知条件和要求
2. 制定策略：选择解决方法
3. 逐步计算：展示每一步的计算过程
4. 验证答案：检查结果是否合理
5. 给出最终答案

请详细展示你的思考过程。"""),
        ("human", "{problem}")
    ])
    cot_chain = cot_prompt | llm_standard | StrOutputParser()
    cot_answer = cot_chain.invoke({"problem": problem})
    print(f"\n回答:\n{cot_answer}")

    print("\n" + "─" * 70)
    print("对比总结")
    print("─" * 70)
    print("""
【Zero-Shot】
- 优点：响应快，适合简单问题
- 缺点：复杂问题容易出错，无法检查中间步骤
- 适用：事实性问题、简单计算

【Chain-of-Thought】
- 优点：
  1. 推理过程透明，便于验证
  2. 复杂问题准确率更高
  3. 出错时可以定位到具体步骤
  4. 可作为教学/解释材料
- 缺点：输出更长，消耗更多token
- 适用：数学问题、逻辑推理、多步骤任务
""")


# ============ 3. Demo 2: Self-Correction 自我修正 ============

class SelfCorrectionAgent:
    """
    自我修正 Agent

    流程：生成初始答案 → 评估质量 → 如有不足则修正 → 重复直到满意
    """

    def __init__(self, max_iterations: int = 3):
        self.max_iterations = max_iterations

    def solve(self, problem: str, verbose: bool = True) -> Dict:
        """
        迭代式解决问题

        Returns:
            Dict 包含 final_answer, iterations, improvement_history
        """
        if verbose:
            print("=" * 70)
            print("Demo 2: Self-Correction (自我修正)")
            print("=" * 70)
            print(f"\n问题: {problem}\n")

        history = []

        # 第1轮：生成初始答案
        current_answer = self._generate_initial(problem)
        history.append({
            "iteration": 1,
            "stage": "初始生成",
            "answer": current_answer
        })

        if verbose:
            print(f"{'─' * 70}")
            print("迭代 1: 初始生成")
            print(f"{'─' * 70}")
            print(f"{current_answer}\n")

        # 后续轮次：评估并修正
        for i in range(2, self.max_iterations + 1):
            # 评估当前答案
            evaluation = self._evaluate(problem, current_answer)

            if verbose:
                print(f"{'─' * 70}")
                print(f"迭代 {i}: 评估与修正")
                print(f"{'─' * 70}")
                print(f"评估:\n{evaluation}\n")

            # 检查是否满意
            if self._is_satisfactory(evaluation):
                if verbose:
                    print("✅ 评估认为答案已足够好，停止迭代\n")
                break

            # 修正答案
            current_answer = self._refine(problem, current_answer, evaluation)
            history.append({
                "iteration": i,
                "stage": "修正",
                "evaluation": evaluation,
                "answer": current_answer
            })

            if verbose:
                print(f"修正后:\n{current_answer}\n")
        else:
            if verbose:
                print(f"⚠️ 达到最大迭代次数 ({self.max_iterations})\n")

        return {
            "final_answer": current_answer,
            "iterations": len(history),
            "history": history
        }

    def _generate_initial(self, problem: str) -> str:
        """生成初始答案"""
        prompt = ChatPromptTemplate.from_messages([
            ("system", "你是一个助手。请回答问题，不需要展示思考过程。"),
            ("human", "{problem}")
        ])
        chain = prompt | llm_standard | StrOutputParser()
        return chain.invoke({"problem": problem})

    def _evaluate(self, problem: str, answer: str) -> str:
        """评估答案质量"""
        prompt = ChatPromptTemplate.from_messages([
            ("system", """你是一个严格的评估专家。请评估以下答案的质量。

评估维度：
1. 准确性：答案是否正确
2. 完整性：是否回答了问题的所有方面
3. 清晰性：表达是否清晰易懂
4. 逻辑性：推理是否有逻辑漏洞

请给出评分（1-10）并详细说明优缺点。
如果答案有不足，请具体指出需要改进的地方。"""),
            ("human", """问题: {problem}

答案: {answer}

请评估这个答案。""")
        ])
        chain = prompt | llm_standard | StrOutputParser()
        return chain.invoke({"problem": problem, "answer": answer})

    def _is_satisfactory(self, evaluation: str) -> bool:
        """根据评估判断是否满意（评分 >= 8 或明确说满意）"""
        # 尝试提取评分
        scores = re.findall(r'(\d+)[/\s]*10', evaluation)
        if scores:
            return int(scores[0]) >= 8
        # 如果没有明确评分，检查是否包含正面关键词
        positive = ["足够好", "满意", "优秀", "完美", "无需改进"]
        return any(p in evaluation for p in positive)

    def _refine(self, problem: str, answer: str, evaluation: str) -> str:
        """根据评估反馈修正答案"""
        prompt = ChatPromptTemplate.from_messages([
            ("system", "你是一个助手。请根据评估反馈改进你的答案。"),
            ("human", """问题: {problem}

当前答案: {answer}

评估反馈: {evaluation}

请根据反馈改进答案，给出更好的版本。""")
        ])
        chain = prompt | llm_standard | StrOutputParser()
        return chain.invoke({
            "problem": problem,
            "answer": answer,
            "evaluation": evaluation
        })


# ============ 4. Demo 3: Program-aided Reasoning ============

def demo_program_aided_reasoning():
    """
    Program-aided Reasoning (程序辅助推理)

    对于需要精确计算的问题，让 LLM 生成 Python 代码并执行，
    然后用执行结果回答问题。这比纯文本推理更可靠。
    """
    print("=" * 70)
    print("Demo 3: Program-aided Reasoning (程序辅助推理)")
    print("=" * 70)

    problem = """
小明去超市买东西：
- 买了3斤苹果，每斤12.5元
- 买了2斤香蕉，每斤8.3元
- 买了1.5斤葡萄，每斤28.6元
- 超市满50元打9折

请问小明需要付多少钱？
"""

    print(f"\n问题: {problem.strip()}\n")

    # --- 纯文本推理 ---
    print("─" * 70)
    print("【方式 A】纯文本推理")
    print("─" * 70)

    text_prompt = ChatPromptTemplate.from_messages([
        ("system", "你是一个数学助手，直接计算并给出答案。"),
        ("human", "{problem}")
    ])
    text_chain = text_prompt | llm_standard | StrOutputParser()
    text_answer = text_chain.invoke({"problem": problem})
    print(f"\n{text_answer}\n")

    # --- 程序辅助推理 ---
    print("─" * 70)
    print("【方式 B】Program-aided Reasoning")
    print("─" * 70)

    # Step 1: 让 LLM 生成 Python 代码
    code_gen_prompt = ChatPromptTemplate.from_messages([
        ("system", """你是一个 Python 编程专家。
请根据问题生成一段可执行的 Python 代码来解决它。
要求：
1. 代码必须完整、可独立运行
2. 代码需要打印最终答案
3. 只输出代码，不要输出解释"""),
        ("human", "{problem}")
    ])
    code_gen_chain = code_gen_prompt | llm_reasoner | StrOutputParser()
    generated_code = code_gen_chain.invoke({"problem": problem})

    # 提取代码块
    code = _extract_code(generated_code)
    print(f"\n生成的代码:\n{'─' * 50}")
    print(code)
    print(f"{'─' * 50}")

    # Step 2: 执行代码
    execution_result = _execute_python_code(code)
    print(f"\n执行结果:\n{execution_result}\n")

    # Step 3: 用执行结果生成最终回答
    final_prompt = ChatPromptTemplate.from_messages([
        ("system", "你是一个友好的助手。请根据代码执行结果，用自然语言回答用户问题。"),
        ("human", """问题: {problem}

代码执行结果: {result}

请用中文给出最终答案。""")
    ])
    final_chain = final_prompt | llm_standard | StrOutputParser()
    final_answer = final_chain.invoke({
        "problem": problem,
        "result": execution_result
    })
    print(f"最终回答:\n{final_answer}\n")

    print("─" * 70)
    print("对比总结")
    print("─" * 70)
    print("""
【纯文本推理】
- 优点：自然流畅，适合概念性问题
- 缺点：计算容易出错，尤其是小数运算
- 局限：无法保证数值精确性

【Program-aided Reasoning】
- 优点：
  1. 计算结果100%准确
  2. 可以处理复杂数学运算
  3. 推理过程可复现、可验证
  4. 出错时可以调试代码
- 缺点：需要代码执行环境，有安全风险
- 适用：数学计算、数据分析、逻辑验证
""")


def _extract_code(text: str) -> str:
    """从 LLM 输出中提取 Python 代码块"""
    # 尝试提取 ```python ... ``` 格式的代码
    match = re.search(r'```python\n(.*?)\n```', text, re.DOTALL)
    if match:
        return match.group(1)
    # 尝试提取 ``` ... ``` 格式的代码
    match = re.search(r'```\n(.*?)\n```', text, re.DOTALL)
    if match:
        return match.group(1)
    # 如果没有代码块标记，尝试提取看起来像是代码的部分
    return text.strip()


def _execute_python_code(code: str) -> str:
    """安全地执行 Python 代码"""
    try:
        result = subprocess.run(
            ["python", "-c", code],
            capture_output=True,
            text=True,
            timeout=10
        )
        if result.returncode == 0:
            return result.stdout.strip() or "(无输出)"
        else:
            return f"执行错误: {result.stderr.strip()}"
    except subprocess.TimeoutExpired:
        return "执行超时"
    except Exception as e:
        return f"执行异常: {str(e)}"


# ============ 5. Demo 4: ReAct 风格多步推理 ============

def demo_react_reasoning():
    """
    ReAct (Reasoning + Acting) 风格推理

    模拟 ReAct 循环：Thought → Action → Observation → Thought...
    本示例中，Action 是调用 Python 计算器工具
    """
    print("=" * 70)
    print("Demo 4: ReAct 风格多步推理")
    print("=" * 70)

    problem = """
一个长方形花园，周长是60米，长是宽的2倍。
请问这个花园的面积是多少平方米？
"""

    print(f"\n问题: {problem.strip()}\n")

    # ReAct 循环
    print("─" * 70)
    print("开始 ReAct 推理循环")
    print("─" * 70)

    conversation = []
    max_steps = 5

    for step in range(1, max_steps + 1):
        print(f"\n🔄 Step {step}")
        print("─" * 50)

        # Thought: 思考下一步
        thought_prompt = ChatPromptTemplate.from_messages([
            ("system", """你是一个解决数学问题的智能体。
请分析当前状态，决定下一步行动。

你可以执行以下行动：
- CALCULATE[expression]: 执行数学计算（如 CALCULATE[60/2]）
- ANSWER[answer]: 给出最终答案

请用以下格式输出：
THOUGHT: 你的思考过程
ACTION: 你的行动
"""),
            ("human", """问题: {problem}

之前的行动记录:
{history}

请决定下一步。""")
        ])
        thought_chain = thought_prompt | llm_standard | StrOutputParser()

        history_str = "\n".join([
            f"Step {h['step']}: {h['thought']} -> {h['action']} -> {h.get('observation', '')}"
            for h in conversation
        ]) if conversation else "(无)"

        response = thought_chain.invoke({
            "problem": problem,
            "history": history_str
        })

        # 解析 Thought 和 Action
        thought_match = re.search(r'THOUGHT:\s*(.*?)(?:\nACTION:|$)', response, re.DOTALL)
        action_match = re.search(r'ACTION:\s*(.*?)(?:\n|$)', response, re.DOTALL)

        thought = thought_match.group(1).strip() if thought_match else response
        action = action_match.group(1).strip() if action_match else "ANSWER[无法解析行动]"

        print(f"🤔 THOUGHT: {thought}")
        print(f"🔧 ACTION: {action}")

        # 执行 Action
        observation = _execute_action(action)
        print(f"👁️  OBSERVATION: {observation}")

        conversation.append({
            "step": step,
            "thought": thought,
            "action": action,
            "observation": observation
        })

        # 检查是否给出最终答案
        if action.startswith("ANSWER["):
            print(f"\n✅ 最终答案: {observation}")
            break
    else:
        print(f"\n⚠️ 达到最大步数 ({max_steps})")

    print("\n" + "─" * 70)
    print("ReAct 推理过程总结")
    print("─" * 70)
    for h in conversation:
        print(f"\nStep {h['step']}:")
        print(f"  思考: {h['thought'][:100]}...")
        print(f"  行动: {h['action']}")
        print(f"  观察: {h['observation'][:100]}...")


def _execute_action(action: str) -> str:
    """执行 ReAct 行动"""
    # CALCULATE[expression]
    calc_match = re.search(r'CALCULATE\[(.*?)\]', action)
    if calc_match:
        expression = calc_match.group(1)
        try:
            result = eval(expression, {"__builtins__": {}}, {})
            return str(result)
        except Exception as e:
            return f"计算错误: {str(e)}"

    # ANSWER[answer]
    answer_match = re.search(r'ANSWER\[(.*?)\]', action)
    if answer_match:
        return answer_match.group(1)

    return "未知行动"


# ============ 6. 综合对比演示 ============

def demo_comprehensive_comparison():
    """
    综合对比：同一个问题用不同推理技术解决
    """
    print("\n" + "=" * 70)
    print("综合对比：四种推理技术解决同一问题")
    print("=" * 70)

    problem = """
一个水箱有进水管和出水管。
进水管单独注满需要8小时，出水管单独排空需要12小时。
如果同时打开进水管和出水管，多久能注满水箱？
"""

    print(f"\n问题: {problem.strip()}\n")

    techniques = [
        ("Zero-Shot", _solve_zero_shot),
        ("Chain-of-Thought", _solve_cot),
        ("Self-Correction", _solve_self_correction),
        ("Program-aided", _solve_program_aided),
    ]

    results = []
    for name, solver in techniques:
        print(f"{'─' * 70}")
        print(f"【{name}】")
        print(f"{'─' * 70}")
        start = time.time()
        answer = solver(problem)
        elapsed = time.time() - start
        print(f"\n答案:\n{answer}")
        print(f"\n⏱️ 耗时: {elapsed:.2f}秒")
        results.append((name, elapsed))

    print(f"\n{'=' * 70}")
    print("技术对比表")
    print(f"{'=' * 70}")
    print(f"{'技术':<20} {'耗时':<10}")
    print("─" * 35)
    for name, elapsed in results:
        print(f"{name:<20} {elapsed:.2f}s")


def _solve_zero_shot(problem: str) -> str:
    """Zero-Shot 求解"""
    prompt = ChatPromptTemplate.from_messages([
        ("system", "直接给出答案，不要解释。"),
        ("human", "{problem}")
    ])
    chain = prompt | llm_standard | StrOutputParser()
    return chain.invoke({"problem": problem})


def _solve_cot(problem: str) -> str:
    """CoT 求解"""
    prompt = ChatPromptTemplate.from_messages([
        ("system", "请逐步思考并解决问题。"),
        ("human", "{problem}")
    ])
    chain = prompt | llm_standard | StrOutputParser()
    return chain.invoke({"problem": problem})


def _solve_self_correction(problem: str) -> str:
    """Self-Correction 求解（简化版，1轮修正）"""
    agent = SelfCorrectionAgent(max_iterations=2)
    result = agent.solve(problem, verbose=False)
    return result["final_answer"]


def _solve_program_aided(problem: str) -> str:
    """Program-aided 求解"""
    code_gen_prompt = ChatPromptTemplate.from_messages([
        ("system", "生成Python代码解决数学问题。只输出代码。"),
        ("human", "{problem}")
    ])
    code = _extract_code((code_gen_prompt | llm_standard | StrOutputParser()).invoke({"problem": problem}))
    result = _execute_python_code(code)

    final_prompt = ChatPromptTemplate.from_messages([
        ("system", "根据计算结果给出最终答案。"),
        ("human", "问题: {problem}\n计算结果: {result}\n给出最终答案。")
    ])
    return (final_prompt | llm_standard | StrOutputParser()).invoke({
        "problem": problem,
        "result": result
    })


# ============ 7. 主函数 ============

def main():
    # Demo 1: Zero-Shot vs CoT
    demo_zero_shot_vs_cot()

    # Demo 2: Self-Correction
    agent = SelfCorrectionAgent(max_iterations=3)
    problem2 = """
请解释量子计算的基本原理，并给出一个潜在的应用场景。
要求回答在200字以内。
"""
    agent.solve(problem2, verbose=True)

    # Demo 3: Program-aided Reasoning
    demo_program_aided_reasoning()

    # Demo 4: ReAct
    demo_react_reasoning()

    # 综合对比
    demo_comprehensive_comparison()

    # 总结
    print("\n" + "=" * 70)
    print("Reasoning Techniques 优势总结")
    print("=" * 70)
    print("""
【核心推理技术对比】

| 技术 | 核心思想 | 适用场景 | 优势 | 局限 |
|------|---------|---------|------|------|
| Zero-Shot | 直接回答 | 简单问题 | 快速、简洁 | 复杂问题易错 |
| Chain-of-Thought | 逐步思考 | 数学/逻辑 | 透明、可验证 | 输出长、token多 |
| Self-Correction | 迭代改进 | 高质量要求 | 持续提升质量 | 多轮调用、成本高 |
| Program-aided | 代码执行 | 精确计算 | 100%准确 | 需要执行环境 |
| ReAct | 推理+行动 | 多步任务 | 动态适应 | 实现复杂 |

【选择指南】

1. **问题简单？** → Zero-Shot 或 CoT
2. **需要精确计算？** → Program-aided Reasoning
3. **答案质量要求高？** → Self-Correction
4. **需要多步交互？** → ReAct
5. **不确定？** → 从 CoT 开始，根据需要升级

【组合使用】

- CoT + Self-Correction: 先逐步思考，再迭代改进
- CoT + Program-aided: 思考过程用自然语言，计算用代码
- ReAct + Tool Use: 推理过程中动态调用外部工具

【与书中模式的关联】

- CoT 是 Prompt Engineering 的核心技巧
- Self-Correction 是 Reflection Pattern 的一种实现
- Program-aided 是 Tool Use Pattern 的特例
- ReAct 是 Planning + Tool Use 的结合
""")


if __name__ == "__main__":
    main()
