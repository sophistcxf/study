"""
Chapter 11: Goal Setting and Monitoring Pattern (目标设定与监控模式) - 示例2

本示例完全基于书中第11章的示例，展示了一个代码生成智能体，
能够根据指定的目标和用例自动生成和优化Python代码。

场景描述（来自书中）：
一个自主AI智能体被设计用来生成和优化Python代码。它的核心功能是：
1. 接收一个编程任务（use case）和一组目标（goals）
2. 生成初始代码
3. 评估代码是否满足所有目标
4. 如果不满足，进行迭代优化
5. 持续监控进度直到所有目标达成

书中的示例：
- Use Case: "Write code to find BinaryGap of a given positive integer"
- Goals: "Code simple to understand, Functionally correct, 
         Handles comprehensive edge cases, Takes positive integer input only,
         prints the results with few examples"

核心概念：
- 目标设定：明确定义代码应该达到的质量标准
- 进度监控：检查每个目标是否达成
- 迭代优化：根据监控结果不断改进代码
- 终止条件：当所有目标都满足时停止
"""

import os
import re
from typing import List, Dict, Optional, Tuple
from dataclasses import dataclass, field
from datetime import datetime
from enum import Enum
from langchain_openai import ChatOpenAI
from langchain_core.prompts import ChatPromptTemplate
from langchain_core.output_parsers import StrOutputParser, JsonOutputParser
from dotenv import load_dotenv

load_dotenv()

# 初始化 LLM
llm = ChatOpenAI(
    model="deepseek-chat",
    temperature=0.2,
    base_url="https://api.deepseek.com/v1",
    api_key=os.getenv("DEEPSEEK_API_KEY")
)


# ============ 1. 数据模型定义 ============

class GoalStatus(Enum):
    """目标状态"""
    NOT_MET = "未达成"
    PARTIALLY_MET = "部分达成"
    MET = "已达成"


@dataclass
class CodeGoal:
    """代码质量目标"""
    id: str
    description: str  # 目标描述
    status: GoalStatus = GoalStatus.NOT_MET
    feedback: str = ""  # 评估反馈
    iteration_met: Optional[int] = None  # 在哪次迭代中达成


@dataclass
class CodeGenerationState:
    """代码生成状态"""
    use_case: str  # 编程任务
    goals: List[CodeGoal]  # 质量目标列表
    current_code: str = ""  # 当前代码
    iteration: int = 0  # 当前迭代次数
    max_iterations: int = 5  # 最大迭代次数
    all_goals_met: bool = False  # 是否所有目标都已达成
    history: List[Dict] = field(default_factory=list)  # 迭代历史
    
    def get_progress(self) -> float:
        """计算总体进度"""
        if not self.goals:
            return 0.0
        met_count = sum(1 for g in self.goals if g.status == GoalStatus.MET)
        return (met_count / len(self.goals)) * 100


# ============ 2. 代码生成功能 ============

def generate_initial_code(use_case: str, goals: List[str]) -> str:
    """
    根据用例和目标生成初始代码
    
    Args:
        use_case: 编程任务描述
        goals: 质量目标列表
    
    Returns:
        生成的Python代码
    """
    prompt = ChatPromptTemplate.from_messages([
        ("system", """你是一个专业的Python程序员。请根据给定的用例编写清晰、正确的代码。
只返回Python代码，不要包含解释或其他内容。"""),
        ("human", """请用Python实现以下功能：

用例：{use_case}

需要满足的目标：
{goals}

请编写完整的Python代码，包括必要的注释和测试示例。""")
    ])
    
    chain = prompt | llm | StrOutputParser()
    code = chain.invoke({
        "use_case": use_case,
        "goals": "\n".join(f"- {g}" for g in goals)
    })
    
    # 清理代码（移除markdown标记）
    code = clean_code(code)
    
    return code


def clean_code(code: str) -> str:
    """清理代码中的markdown标记"""
    # 移除 ```python 和 ``` 标记
    code = re.sub(r'```python\s*', '', code)
    code = re.sub(r'```\s*', '', code)
    return code.strip()


# ============ 3. 目标评估功能 ============

def evaluate_code_against_goals(code: str, goals: List[CodeGoal], use_case: str) -> List[CodeGoal]:
    """
    评估代码是否满足各个目标
    
    Args:
        code: 待评估的代码
        goals: 目标列表
        use_case: 原始用例
    
    Returns:
        更新后的目标列表（包含评估结果）
    """
    # 为每个目标创建评估prompt
    evaluated_goals = []
    
    for goal in goals:
        prompt = ChatPromptTemplate.from_messages([
            ("system", """你是一个代码审查专家。请评估给定的代码是否满足特定目标。
回答格式：
status: MET/PARTIALLY_MET/NOT_MET
feedback: 详细的评估反馈"""),
            ("human", """请评估以下代码是否满足目标：

目标：{goal_description}

用例：{use_case}

代码：
{code}

请给出评估结果。""")
        ])
        
        chain = prompt | llm | StrOutputParser()
        result = chain.invoke({
            "goal_description": goal.description,
            "use_case": use_case,
            "code": code
        })
        
        # 解析结果
        status = GoalStatus.NOT_MET
        feedback = result
        
        if "MET" in result.upper():
            if "PARTIAL" in result.upper():
                status = GoalStatus.PARTIALLY_MET
            else:
                status = GoalStatus.MET
        
        evaluated_goal = CodeGoal(
            id=goal.id,
            description=goal.description,
            status=status,
            feedback=feedback
        )
        evaluated_goals.append(evaluated_goal)
    
    return evaluated_goals


# ============ 4. 代码优化功能 ============

def refine_code(current_code: str, unmet_goals: List[CodeGoal], use_case: str) -> str:
    """
    根据未达成的目标优化代码
    
    Args:
        current_code: 当前代码
        unmet_goals: 未达成的目标列表
        use_case: 原始用例
    
    Returns:
        优化后的代码
    """
    unmet_descriptions = "\n".join([
        f"- {g.description}\n  反馈: {g.feedback[:200]}"
        for g in unmet_goals
    ])
    
    prompt = ChatPromptTemplate.from_messages([
        ("system", """你是一个代码优化专家。请根据反馈意见改进代码，使其更好地满足要求。
只返回优化后的完整Python代码，不要包含解释。"""),
        ("human", """请优化以下代码，以更好地满足这些目标：

需要改进的目标：
{unmet_goals}

原始用例：{use_case}

当前代码：
{current_code}

请提供优化后的完整代码。""")
    ])
    
    chain = prompt | llm | StrOutputParser()
    refined_code = chain.invoke({
        "unmet_goals": unmet_descriptions,
        "use_case": use_case,
        "current_code": current_code
    })
    
    # 清理代码
    refined_code = clean_code(refined_code)
    
    return refined_code


# ============ 5. 主循环：目标设定与监控 ============

def run_code_agent(use_case: str, goals_input: str, max_iterations: int = 5) -> CodeGenerationState:
    """
    运行代码生成智能体（书中的核心示例）
    
    Args:
        use_case: 编程任务描述
        goals_input: 目标描述（用逗号分隔）
        max_iterations: 最大迭代次数
    
    Returns:
        代码生成状态
    """
    print(f"\n{'='*80}")
    print(f"🚀 Chapter 11: Goal Setting and Monitoring Pattern")
    print(f"{'='*80}\n")
    print(f"📋 用例: {use_case}")
    print(f"🎯 目标:")
    
    # 解析目标
    goals_list = [g.strip() for g in goals_input.split(",") if g.strip()]
    for i, goal_desc in enumerate(goals_list, 1):
        print(f"   {i}. {goal_desc}")
    
    # 创建目标对象
    goals = [CodeGoal(id=f"G-{i+1:02d}", description=desc) 
             for i, desc in enumerate(goals_list)]
    
    # 初始化状态
    state = CodeGenerationState(
        use_case=use_case,
        goals=goals,
        max_iterations=max_iterations
    )
    
    print(f"\n{'='*80}")
    print("开始代码生成与优化循环")
    print(f"{'='*80}\n")
    
    # 主循环：生成 → 评估 → 优化
    while state.iteration < state.max_iterations and not state.all_goals_met:
        state.iteration += 1
        print(f"\n【迭代 {state.iteration}】")
        print(f"{'-'*80}")
        
        # 步骤1：生成或优化代码
        if state.iteration == 1:
            print("📝 生成初始代码...")
            code = generate_initial_code(use_case, goals_list)
        else:
            # 找出未达成的目标
            unmet_goals = [g for g in state.goals if g.status != GoalStatus.MET]
            print(f"🔧 优化代码（{len(unmet_goals)}个目标未达成）...")
            code = refine_code(state.current_code, unmet_goals, use_case)
        
        state.current_code = code
        
        # 步骤2：评估代码
        print("✅ 评估代码质量...")
        evaluated_goals = evaluate_code_against_goals(code, state.goals, use_case)
        state.goals = evaluated_goals
        
        # 步骤3：更新状态
        for goal in state.goals:
            if goal.status == GoalStatus.MET and goal.iteration_met is None:
                goal.iteration_met = state.iteration
        
        # 检查是否所有目标都已达成
        state.all_goals_met = all(g.status == GoalStatus.MET for g in state.goals)
        
        # 记录历史
        state.history.append({
            "iteration": state.iteration,
            "goals_status": [(g.id, g.status.value) for g in state.goals],
            "progress": state.get_progress()
        })
        
        # 打印本次迭代的结果
        print(f"\n📊 目标达成情况:")
        for goal in state.goals:
            status_icon = {"已达成": "✅", "部分达成": "⚠️ ", "未达成": "❌"}.get(
                goal.status.value, "❓"
            )
            print(f"   {status_icon} [{goal.id}] {goal.description[:50]}")
            if goal.feedback:
                print(f"       反馈: {goal.feedback[:100]}...")
        
        progress = state.get_progress()
        print(f"\n📈 总体进度: {progress:.1f}%")
        
        if state.all_goals_met:
            print(f"\n✨ 所有目标已达成！")
            break
        elif state.iteration >= state.max_iterations:
            print(f"\n⚠️  达到最大迭代次数 ({max_iterations})")
    
    return state


# ============ 6. 结果展示 ============

def print_final_results(state: CodeGenerationState):
    """
    打印最终结果
    
    Args:
        state: 代码生成状态
    """
    print(f"\n{'='*80}")
    print(f"📊 最终结果汇总")
    print(f"{'='*80}\n")
    
    print(f"📋 用例: {state.use_case}")
    print(f"🔄 总迭代次数: {state.iteration}")
    print(f"✅ 所有目标达成: {'是' if state.all_goals_met else '否'}")
    print(f"📈 最终进度: {state.get_progress():.1f}%\n")
    
    print(f"🎯 目标达成详情:")
    for goal in state.goals:
        status_icon = {"已达成": "✅", "部分达成": "⚠️ ", "未达成": "❌"}.get(
            goal.status.value, "❓"
        )
        iteration_info = f"(在第{goal.iteration_met}次迭代达成)" if goal.iteration_met else ""
        print(f"   {status_icon} [{goal.id}] {goal.description}")
        print(f"       状态: {goal.status.value} {iteration_info}")
    
    print(f"\n💻 最终代码:")
    print(f"{'-'*80}")
    print(state.current_code)
    print(f"{'-'*80}\n")
    
    print(f"📈 迭代历史:")
    for record in state.history:
        print(f"   迭代 {record['iteration']}: 进度 {record['progress']:.1f}%")
        for goal_id, status in record['goals_status']:
            print(f"      - {goal_id}: {status}")


# ============ 7. 书中示例 ============

def run_book_example_1():
    """
    运行书中的示例1：BinaryGap问题
    （这是书中第11章的具体示例）
    """
    print("\n" + "="*80)
    print("📖 运行书中的示例1：BinaryGap问题")
    print("="*80)
    
    use_case = "Write code to find BinaryGap of a given positive integer"
    goals = ("Code simple to understand, Functionally correct, "
             "Handles comprehensive edge cases, Takes positive integer input only, "
             "prints the results with few examples")
    
    state = run_code_agent(use_case, goals, max_iterations=5)
    print_final_results(state)
    
    return state


def run_book_example_2():
    """
    运行书中的示例2：文件计数问题
    """
    print("\n" + "="*80)
    print("📖 运行书中的示例2：文件计数问题")
    print("="*80)
    
    use_case = "Write code to count the number of files in a directory recursively"
    goals = ("Code is efficient, Handles permission errors gracefully, "
             "Returns accurate count, Includes file type filtering option, "
             "Has clear documentation")
    
    state = run_code_agent(use_case, goals, max_iterations=5)
    print_final_results(state)
    
    return state


# ============ 主程序入口 ============

if __name__ == "__main__":
    import sys
    
    if len(sys.argv) > 1:
        if sys.argv[1] == "example1":
            run_book_example_1()
        elif sys.argv[1] == "example2":
            run_book_example_2()
        else:
            print("用法:")
            print("  python exp2.py example1    # BinaryGap示例（书中示例1）")
            print("  python exp2.py example2    # 文件计数示例（书中示例2）")
    else:
        # 默认运行书中的第一个示例
        run_book_example_1()
