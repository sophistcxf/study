"""
Chapter 4: Reflection Pattern (反思模式)

反思模式是 Agentic Design Patterns 中的高级模式。
它让 LLM 自我评估输出，识别问题并改进，通过迭代提升质量。

核心思想：
- LLM 生成初始输出
- LLM 对自己的输出进行批判性评估
- 根据评估结果改进输出
- 可以多次迭代直到满足质量标准

两种主要形式：
1. Self-Reflection（自我反思）：同一模型生成和评估
2. Multi-Agent Reflection（多智能体反思）：一个生成，另一个评估

示例场景：代码生成与优化，通过反思不断改进代码质量
"""

import os
from typing import Optional
from langchain_openai import ChatOpenAI
from langchain_core.prompts import ChatPromptTemplate
from langchain_core.output_parsers import StrOutputParser
from dotenv import load_dotenv

load_dotenv()

# 初始化 LLM
llm = ChatOpenAI(
    model="deepseek-chat",
    temperature=0,
    base_url="https://api.deepseek.com/v1",
    api_key=os.getenv("DEEPSEEK_API_KEY")
)


# ============ 1. 基础组件 ============

def generate_code(requirement: str) -> str:
    """根据需求生成代码"""
    prompt = ChatPromptTemplate.from_messages([
        ("system", """你是一个资深 Python 开发工程师。
请根据需求编写清晰、高效、可维护的代码。
代码应包含适当的注释和文档字符串。"""),
        ("human", "请编写代码实现以下需求：\n\n{requirement}")
    ])
    chain = prompt | llm | StrOutputParser()
    return chain.invoke({"requirement": requirement})


def reflect_on_code(requirement: str, code: str) -> str:
    """对代码进行反思评估"""
    prompt = ChatPromptTemplate.from_messages([
        ("system", """你是一个严格的代码审查专家。请从以下维度评估代码：

1. **正确性**：代码是否正确实现了需求？
2. **健壮性**：是否处理了边界情况和异常？
3. **可读性**：代码是否清晰易懂？命名是否规范？
4. **效率**：算法复杂度是否合理？
5. **Pythonic**：是否符合 Python 最佳实践？

请列出所有发现的问题，并按严重程度排序。
如果没有问题，请明确说明"代码质量良好，无需改进"。"""),
        ("human", """需求：
{requirement}

代码：
```python
{code}
```

请进行代码审查：""")
    ])
    chain = prompt | llm | StrOutputParser()
    return chain.invoke({"requirement": requirement, "code": code})


def improve_code(requirement: str, code: str, feedback: str) -> str:
    """根据反馈改进代码"""
    prompt = ChatPromptTemplate.from_messages([
        ("system", """你是一个资深 Python 开发工程师。
请根据代码审查反馈，改进代码并解决所有问题。
保持代码的清晰性和可维护性。"""),
        ("human", """需求：
{requirement}

原始代码：
```python
{code}
```

审查反馈：
{feedback}

请提供改进后的完整代码：""")
    ])
    chain = prompt | llm | StrOutputParser()
    return chain.invoke({
        "requirement": requirement,
        "code": code,
        "feedback": feedback
    })


# ============ 2. Self-Reflection 模式 ============

def self_reflection_loop(
    requirement: str,
    max_iterations: int = 3,
    verbose: bool = True
) -> dict:
    """
    自我反思循环：生成 -> 反思 -> 改进 -> 再反思...
    """
    if verbose:
        print("=" * 70)
        print("Self-Reflection 模式：代码生成与优化")
        print("=" * 70)
        print(f"\n需求: {requirement}\n")

    # 初始生成
    current_code = generate_code(requirement)

    if verbose:
        print("─" * 70)
        print("第 1 轮：初始生成")
        print("─" * 70)
        print(current_code[:800] + "...\n" if len(current_code) > 800 else current_code + "\n")

    history = [{
        "iteration": 1,
        "code": current_code,
        "feedback": None,
        "action": "generate"
    }]

    # 反思-改进循环
    for iteration in range(2, max_iterations + 1):
        # 反思
        feedback = reflect_on_code(requirement, current_code)

        if verbose:
            print("─" * 70)
            print(f"第 {iteration} 轮：反思评估")
            print("─" * 70)
            print(feedback[:600] + "...\n" if len(feedback) > 600 else feedback + "\n")

        # 检查是否通过
        if "无需改进" in feedback or "代码质量良好" in feedback:
            if verbose:
                print(f"✅ 代码质量已达标，停止迭代（第 {iteration-1} 轮）\n")
            history.append({
                "iteration": iteration,
                "code": current_code,
                "feedback": feedback,
                "action": "pass"
            })
            break

        # 改进
        improved_code = improve_code(requirement, current_code, feedback)

        if verbose:
            print("─" * 70)
            print(f"第 {iteration} 轮：代码改进")
            print("─" * 70)
            print(improved_code[:800] + "...\n" if len(improved_code) > 800 else improved_code + "\n")

        history.append({
            "iteration": iteration,
            "code": improved_code,
            "feedback": feedback,
            "action": "improve"
        })

        current_code = improved_code

    return {
        "final_code": current_code,
        "iterations": len(history),
        "history": history
    }


# ============ 3. Multi-Agent Reflection 模式 ============

class CodeGenerator:
    """代码生成智能体"""

    def __init__(self):
        self.prompt = ChatPromptTemplate.from_messages([
            ("system", """你是一个资深 Python 开发工程师（代码生成专家）。
你的任务是编写高质量的代码。
当收到反馈时，请认真考虑并改进代码。"""),
            ("human", "{instruction}")
        ])
        self.chain = self.prompt | llm | StrOutputParser()

    def generate(self, requirement: str) -> str:
        """生成初始代码"""
        instruction = f"请编写代码实现以下需求：\n\n{requirement}"
        return self.chain.invoke({"instruction": instruction})

    def revise(self, requirement: str, code: str, critique: str) -> str:
        """根据审查意见修改代码"""
        instruction = f"""请根据审查意见修改代码。

需求：
{requirement}

当前代码：
```python
{code}
```

审查意见：
{critique}

请提供改进后的完整代码："""
        return self.chain.invoke({"instruction": instruction})


class CodeReviewer:
    """代码审查智能体"""

    def __init__(self):
        self.prompt = ChatPromptTemplate.from_messages([
            ("system", """你是一个严格的代码审查专家（代码审查员）。
你的任务是找出代码中的所有问题，包括：
- 功能缺陷
- 边界情况未处理
- 代码风格问题
- 性能问题
- 安全漏洞

请详细列出所有问题，并给出具体改进建议。"""),
            ("human", "{instruction}")
        ])
        self.chain = self.prompt | llm | StrOutputParser()

    def review(self, requirement: str, code: str) -> str:
        """审查代码"""
        instruction = f"""请审查以下代码：

需求：
{requirement}

代码：
```python
{code}
```

请进行详细审查："""
        return self.chain.invoke({"instruction": instruction})


def multi_agent_reflection(
    requirement: str,
    max_iterations: int = 3,
    verbose: bool = True
) -> dict:
    """
    多智能体反思：生成器和审查员协作
    """
    if verbose:
        print("\n" + "=" * 70)
        print("Multi-Agent Reflection 模式：生成器 + 审查员")
        print("=" * 70)
        print(f"\n需求: {requirement}\n")

    generator = CodeGenerator()
    reviewer = CodeReviewer()

    # 初始生成
    current_code = generator.generate(requirement)

    if verbose:
        print("─" * 70)
        print("📝 生成器：初始代码")
        print("─" * 70)
        print(current_code[:800] + "...\n" if len(current_code) > 800 else current_code + "\n")

    history = []

    for iteration in range(1, max_iterations):
        # 审查员审查
        critique = reviewer.review(requirement, current_code)

        if verbose:
            print("─" * 70)
            print(f"🔍 审查员：第 {iteration} 轮审查")
            print("─" * 70)
            print(critique[:600] + "...\n" if len(critique) > 600 else critique + "\n")

        # 检查是否通过
        if "无需改进" in critique or "代码质量良好" in critique or len(critique) < 50:
            if verbose:
                print(f"✅ 审查通过，停止迭代\n")
            history.append({
                "iteration": iteration,
                "role": "reviewer",
                "content": critique,
                "action": "pass"
            })
            break

        history.append({
            "iteration": iteration,
            "role": "reviewer",
            "content": critique,
            "action": "review"
        })

        # 生成器修改
        revised_code = generator.revise(requirement, current_code, critique)

        if verbose:
            print("─" * 70)
            print(f"📝 生成器：第 {iteration} 轮修改")
            print("─" * 70)
            print(revised_code[:800] + "...\n" if len(revised_code) > 800 else revised_code + "\n")

        history.append({
            "iteration": iteration,
            "role": "generator",
            "content": revised_code,
            "action": "revise"
        })

        current_code = revised_code

    return {
        "final_code": current_code,
        "iterations": len(history) // 2 + 1,
        "history": history
    }


# ============ 4. 对比：无反思的简单方式 ============

def simple_generate(requirement: str) -> str:
    """简单方式：一次性生成，无反思"""
    return generate_code(requirement)


# ============ 5. 演示 ============

def main():
    # 测试需求
    test_requirement = """
编写一个函数 `parse_user_input`，解析用户输入的日期字符串。

要求：
1. 支持多种格式："2024-01-15", "15/01/2024", "Jan 15, 2024"
2. 返回统一的 datetime 对象
3. 处理无效输入，抛出适当的异常
4. 包含输入验证
"""

    print("=" * 70)
    print("Reflection Pattern 示例 - 代码生成与优化")
    print("=" * 70)

    # Self-Reflection 模式
    self_result = self_reflection_loop(
        requirement=test_requirement,
        max_iterations=3,
        verbose=True
    )

    # Multi-Agent Reflection 模式
    multi_result = multi_agent_reflection(
        requirement=test_requirement,
        max_iterations=3,
        verbose=True
    )

    # 对比简单方式
    print("\n" + "=" * 70)
    print("对比：简单方式（无反思）")
    print("=" * 70)

    simple_code = simple_generate(test_requirement)
    print(f"\n{simple_code[:1000]}...")

    # 总结
    print("\n" + "=" * 70)
    print("Reflection Pattern 优势总结")
    print("=" * 70)
    print(f"""
【实验结果】

Self-Reflection 模式：
- 迭代次数: {self_result['iterations']}
- 特点: 同一模型交替扮演生成者和审查者

Multi-Agent Reflection 模式：
- 迭代次数: {multi_result['iterations']}
- 特点: 两个独立智能体协作

【核心优势】

1. **质量提升**
   - 通过迭代改进，代码质量显著提高
   - 能发现一次性生成时遗漏的问题

2. **自我纠错**
   - 不依赖外部反馈，自主发现问题
   - 减少人工审查的工作量

3. **灵活性**
   - 可以设置停止条件（如质量达标）
   - 可以控制最大迭代次数

4. **可解释性**
   - 每次改进都有明确的理由
   - 便于追踪代码演变过程

【两种模式对比】

| 模式 | 优点 | 缺点 |
|------|------|------|
| Self-Reflection | 简单、成本低 | 可能陷入固定思维模式 |
| Multi-Agent | 视角更多样、审查更严格 | 复杂度更高 |

【适用场景】
- 代码生成与优化
- 文章写作与润色
- 复杂问题求解
- 创意内容生成
- 任何需要高质量输出的任务
""")


if __name__ == "__main__":
    main()
