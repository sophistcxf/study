"""
Chapter 6: Planning Pattern (规划模式)

规划模式是 Agentic Design Patterns 中的高级模式。
它让 LLM 能够将复杂任务分解为可执行的步骤序列，并按计划执行。

核心思想：
- 面对复杂任务时，先制定详细的执行计划
- 将大任务分解为多个小任务（子目标）
- 按顺序或依赖关系执行各个子任务
- 根据执行结果动态调整计划

两种主要形式：
1. 静态规划：一次性制定完整计划，然后执行
2. 动态规划（RePlan）：每执行一步后，根据结果重新规划下一步

示例场景：数据分析报告生成，需要多步骤数据处理和分析
"""

import os
import json
from typing import List, Dict, Optional, Callable
from dataclasses import dataclass, field
from enum import Enum
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


# ============ 1. 任务和计划定义 ============

class TaskStatus(Enum):
    PENDING = "pending"
    IN_PROGRESS = "in_progress"
    COMPLETED = "completed"
    FAILED = "failed"


@dataclass
class Task:
    """任务定义"""
    id: str
    description: str
    status: TaskStatus = TaskStatus.PENDING
    result: Optional[str] = None
    dependencies: List[str] = field(default_factory=list)


@dataclass
class Plan:
    """计划定义"""
    goal: str
    tasks: List[Task]
    current_task_index: int = 0

    def get_next_pending_task(self) -> Optional[Task]:
        """获取下一个待执行的任务"""
        for task in self.tasks:
            if task.status == TaskStatus.PENDING:
                # 检查依赖是否完成
                deps_completed = all(
                    self.get_task_by_id(dep).status == TaskStatus.COMPLETED
                    for dep in task.dependencies
                )
                if deps_completed:
                    return task
        return None

    def get_task_by_id(self, task_id: str) -> Optional[Task]:
        """根据ID获取任务"""
        for task in self.tasks:
            if task.id == task_id:
                return task
        return None

    def is_complete(self) -> bool:
        """检查计划是否完成"""
        return all(task.status == TaskStatus.COMPLETED for task in self.tasks)

    def to_dict(self) -> Dict:
        """转换为字典"""
        return {
            "goal": self.goal,
            "tasks": [
                {
                    "id": t.id,
                    "description": t.description,
                    "status": t.status.value,
                    "result": t.result,
                    "dependencies": t.dependencies
                }
                for t in self.tasks
            ],
            "progress": f"{sum(1 for t in self.tasks if t.status == TaskStatus.COMPLETED)}/{len(self.tasks)}"
        }


# ============ 2. 规划器 ============

class Planner:
    """计划生成器"""

    def __init__(self):
        # 使用简单的字符串模板，避免复杂的格式
        system_template = (
            "你是一个任务规划专家。请将用户的目标分解为具体的执行步骤。\n\n"
            "要求：\n"
            "1. 每个步骤应该是具体、可执行的任务\n"
            "2. 明确步骤之间的依赖关系\n"
            "3. 步骤数量适中（3-8个步骤）\n"
            "4. 使用 JSON 格式输出，包含 tasks 数组\n"
            "5. 每个任务包含 id, description, dependencies 字段\n\n"
            "注意：dependencies 是依赖的任务ID列表。"
        )
        self.prompt = ChatPromptTemplate.from_messages([
            ("system", system_template),
            ("human", "请为以下目标制定执行计划：\n\n{goal}")
        ])
        self.chain = self.prompt | llm | StrOutputParser()

    def create_plan(self, goal: str) -> Plan:
        """创建执行计划"""
        response = self.chain.invoke({"goal": goal})

        # 解析 JSON
        try:
            # 提取 JSON 部分
            json_match = response[response.find("{"):response.rfind("}") + 1]
            data = json.loads(json_match)

            tasks = []
            for task_data in data.get("tasks", []):
                tasks.append(Task(
                    id=task_data["id"],
                    description=task_data["description"],
                    dependencies=task_data.get("dependencies", [])
                ))

            return Plan(goal=goal, tasks=tasks)
        except Exception as e:
            # 如果解析失败，创建默认计划
            return Plan(
                goal=goal,
                tasks=[
                    Task(id="1", description=f"分析目标: {goal}"),
                    Task(id="2", description="执行任务"),
                    Task(id="3", description="总结结果")
                ]
            )


class RePlanner:
    """动态重规划器"""

    def __init__(self):
        system_msg = """你是一个动态规划专家。根据当前执行状态，决定下一步行动。

你可以：
1. 继续执行下一个计划中的任务
2. 调整计划（添加、删除或修改任务）
3. 如果目标已达成，结束执行

请分析当前状态，给出下一步建议。"""
        human_msg = """原始目标: {goal}

当前计划状态:
{plan_state}

上一步执行结果:
{last_result}

请决定下一步行动："""
        self.prompt = ChatPromptTemplate.from_messages([
            ("system", system_msg),
            ("human", human_msg)
        ])
        self.chain = self.prompt | llm | StrOutputParser()

    def replan(self, plan: Plan, last_result: str) -> str:
        """根据执行结果重新规划"""
        plan_state = json.dumps(plan.to_dict(), ensure_ascii=False, indent=2)
        return self.chain.invoke({
            "goal": plan.goal,
            "plan_state": plan_state,
            "last_result": last_result
        })


# ============ 3. 执行器 ============

class TaskExecutor:
    """任务执行器"""

    def __init__(self):
        system_msg = """你是一个任务执行专家。请完成给定的任务，并返回执行结果。

要求：
1. 专注于当前任务
2. 返回清晰的执行结果
3. 如果任务涉及代码，提供完整可运行的代码
4. 如果任务涉及分析，提供详细的分析结论"""
        human_msg = """总体目标: {goal}

当前任务: {task_description}

上下文信息:
{context}

请执行任务并返回结果："""
        self.prompt = ChatPromptTemplate.from_messages([
            ("system", system_msg),
            ("human", human_msg)
        ])
        self.chain = self.prompt | llm | StrOutputParser()

    def execute(self, task: Task, plan: Plan) -> str:
        """执行单个任务"""
        # 收集上下文（已完成的任务结果）
        context = []
        for t in plan.tasks:
            if t.status == TaskStatus.COMPLETED and t.result:
                context.append(f"任务 {t.id} ({t.description}): {t.result[:200]}...")

        context_str = "\n".join(context) if context else "无"

        result = self.chain.invoke({
            "goal": plan.goal,
            "task_description": task.description,
            "context": context_str
        })

        return result


# ============ 4. 规划模式 Agent ============

class PlanningAgent:
    """
    规划模式 Agent
    支持静态规划和动态重规划
    """

    def __init__(self, use_replan: bool = False):
        self.planner = Planner()
        self.replanner = RePlanner() if use_replan else None
        self.executor = TaskExecutor()
        self.use_replan = use_replan

    def run(self, goal: str, verbose: bool = True) -> Dict:
        """执行规划并完成任务"""
        if verbose:
            print("=" * 70)
            print("Planning Pattern 示例 - 智能任务规划器")
            print("=" * 70)
            print(f"\n目标: {goal}\n")

        # 步骤1：创建计划
        if verbose:
            print("─" * 70)
            print("步骤 1: 制定计划")
            print("─" * 70)

        plan = self.planner.create_plan(goal)

        if verbose:
            print(f"\n📋 生成计划（共 {len(plan.tasks)} 个任务）：")
            for task in plan.tasks:
                deps = f" [依赖: {', '.join(task.dependencies)}]" if task.dependencies else ""
                print(f"  {task.id}. {task.description}{deps}")
            print()

        # 步骤2：执行计划
        if verbose:
            print("─" * 70)
            print("步骤 2: 执行计划")
            print("─" * 70)

        iteration = 0
        max_iterations = 20

        while not plan.is_complete() and iteration < max_iterations:
            iteration += 1

            # 获取下一个任务
            task = plan.get_next_pending_task()

            if not task:
                if verbose:
                    print("\n⏸️  等待依赖任务完成...")
                break

            if verbose:
                print(f"\n▶️  执行任务 {task.id}: {task.description}")

            # 执行任务
            task.status = TaskStatus.IN_PROGRESS
            result = self.executor.execute(task, plan)
            task.result = result
            task.status = TaskStatus.COMPLETED

            if verbose:
                print(f"✅ 完成")
                print(f"结果预览: {result[:300]}...\n" if len(result) > 300 else f"结果: {result}\n")

            # 动态重规划
            if self.use_replan and self.replanner:
                replan_suggestion = self.replanner.replan(plan, result)
                if verbose:
                    print(f"🔄 重规划建议: {replan_suggestion[:200]}...\n")

        # 步骤3：总结结果
        if verbose:
            print("─" * 70)
            print("步骤 3: 总结结果")
            print("─" * 70)

        summary = self._summarize_results(plan)

        if verbose:
            print(f"\n{summary}\n")

        return {
            "goal": goal,
            "plan": plan.to_dict(),
            "summary": summary,
            "completed": plan.is_complete()
        }

    def _summarize_results(self, plan: Plan) -> str:
        """总结执行结果"""
        system_msg = "你是一个结果汇总专家。请根据任务执行结果，给出最终总结。"
        human_msg = """原始目标: {goal}

任务执行结果:
{task_results}

请给出最终总结："""
        prompt = ChatPromptTemplate.from_messages([
            ("system", system_msg),
            ("human", human_msg)
        ])
        chain = prompt | llm | StrOutputParser()

        task_results = "\n".join([
            f"任务 {t.id}: {t.description}\n结果: {t.result[:300]}..."
            for t in plan.tasks if t.result
        ])

        return chain.invoke({
            "goal": plan.goal,
            "task_results": task_results
        })


# ============ 5. 对比：无规划的直接执行 ============

def direct_execution(goal: str) -> str:
    """直接执行，无规划"""
    prompt = ChatPromptTemplate.from_messages([
        ("system", "你是一个任务执行助手。请直接完成用户的任务。"),
        ("human", "{goal}")
    ])
    chain = prompt | llm | StrOutputParser()
    return chain.invoke({"goal": goal})


# ============ 6. 演示 ============

def main():
    # 测试任务
    test_goal = """
请帮我完成以下数据分析任务：
1. 分析 ch1/exp1.py 文件的功能和结构
2. 提取其中的核心设计模式概念
3. 与 ch2/exp1.py 进行对比分析
4. 总结两个示例的异同和适用场景
5. 给出学习建议
"""

    print("=" * 70)
    print("Planning Pattern 示例 - 智能任务规划器")
    print("=" * 70)

    # 静态规划模式
    print("\n" + "=" * 70)
    print("模式一：静态规划（一次性制定计划）")
    print("=" * 70)

    static_agent = PlanningAgent(use_replan=False)
    static_result = static_agent.run(test_goal, verbose=True)

    # 动态规划模式
    print("\n" + "=" * 70)
    print("模式二：动态规划（执行中调整）")
    print("=" * 70)

    dynamic_agent = PlanningAgent(use_replan=True)
    dynamic_result = dynamic_agent.run(test_goal, verbose=True)

    # 对比直接执行
    print("\n" + "=" * 70)
    print("对比：直接执行（无规划）")
    print("=" * 70)

    print("\n直接执行结果（前1000字符）：\n")
    direct_result = direct_execution(test_goal)
    print(direct_result[:1000] + "...")

    # 总结
    print("\n" + "=" * 70)
    print("Planning Pattern 优势总结")
    print("=" * 70)
    print("""
【核心优势】

1. **任务分解**
   - 将复杂任务分解为可管理的小任务
   - 降低认知复杂度
   - 每个步骤目标明确

2. **可追踪性**
   - 清晰的执行路径
   - 可以查看每个步骤的结果
   - 便于调试和优化

3. **灵活性**
   - 支持任务依赖关系
   - 动态规划可以适应变化
   - 失败的任务可以单独重试

4. **质量提升**
   - 每个子任务专注单一目标
   - 中间结果可以被后续步骤使用
   - 最终输出更完整、更系统

【两种模式对比】

| 模式 | 优点 | 缺点 | 适用场景 |
|------|------|------|----------|
| 静态规划 | 简单、可预测 | 缺乏灵活性 | 任务明确、环境稳定 |
| 动态规划 | 适应性强 | 复杂度高 | 任务不确定、需要探索 |

【适用场景】

- 数据分析报告生成
- 复杂代码开发
- 多步骤数据处理
- 研究性任务
- 项目管理
- 自动化工作流

【与其他模式结合】

- + Tool Use: 每个计划步骤调用工具执行
- + Reflection: 每步执行后反思改进
- + Parallelization: 无依赖的任务并行执行
- + Routing: 不同类型任务路由到不同执行器
""")


if __name__ == "__main__":
    main()
