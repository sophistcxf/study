"""
Chapter 20: Prioritization (优先级排序模式)

优先级排序是 Agentic Design Patterns 中用于任务管理和资源分配的核心模式。
在复杂、动态的环境中，智能体经常面临多个潜在行动、冲突目标和有限资源。
如果没有明确的优先级确定过程，智能体可能变得低效且无法完成关键目标。

核心思想：
- 不是所有任务都同等重要
- 根据紧急性、重要性、依赖关系、资源成本等标准评估任务
- 动态重新排序允许智能体适应变化的环境
- 优先级可以发生在多个层次：目标级、子任务级、行动级

书中示例场景：项目管理助手，自动创建任务、评估优先级、分配给合适的人员

本示例实现：
1. 任务管理系统（创建、更新、查询）
2. LLM 驱动的优先级评估器
3. 智能资源分配器（根据优先级和人员负载分配）
4. 动态重新排序（应对突发紧急任务）
5. 对比：无优先级 vs 有优先级的系统效率
"""

import os
import time
import random
from typing import List, Dict, Optional
from dataclasses import dataclass, field
from enum import Enum
from collections import defaultdict

from langchain_openai import ChatOpenAI
from langchain_core.prompts import ChatPromptTemplate
from langchain_core.output_parsers import StrOutputParser
from dotenv import load_dotenv

load_dotenv()


# ============ 1. 数据模型 ============

class PriorityLevel(Enum):
    """优先级等级"""
    P0 = "P0"  # 最高优先级：紧急、关键
    P1 = "P1"  # 中等优先级：重要但非紧急
    P2 = "P2"  # 低优先级：可延后


class TaskStatus(Enum):
    """任务状态"""
    PENDING = "pending"
    IN_PROGRESS = "in_progress"
    COMPLETED = "completed"
    BLOCKED = "blocked"


@dataclass
class Task:
    """任务"""
    id: str
    description: str
    priority: PriorityLevel = PriorityLevel.P1
    status: TaskStatus = TaskStatus.PENDING
    assigned_to: Optional[str] = None
    urgency: int = 5       # 1-10，越高越紧急
    importance: int = 5    # 1-10，越高越重要
    estimated_hours: float = 1.0
    dependencies: List[str] = field(default_factory=list)
    created_at: float = field(default_factory=time.time)

    def score(self) -> float:
        """计算任务综合评分（用于排序）"""
        return self.urgency * 0.4 + self.importance * 0.4 + (10 - self.estimated_hours) * 0.2

    def __repr__(self):
        return (f"Task({self.id}, {self.priority.value}, "
                f"urgency={self.urgency}, importance={self.importance}, "
                f"assigned={self.assigned_to or 'None'})")


@dataclass
class Worker:
    """工作人员"""
    name: str
    skills: List[str] = field(default_factory=list)
    max_capacity: int = 3           # 同时处理的最大任务数
    current_tasks: List[str] = field(default_factory=list)

    def is_available(self) -> bool:
        return len(self.current_tasks) < self.max_capacity

    def workload(self) -> float:
        return len(self.current_tasks) / self.max_capacity


# ============ 2. LLM 配置 ============

llm = ChatOpenAI(
    model="deepseek-chat",
    temperature=0.3,
    base_url="https://api.deepseek.com/v1",
    api_key=os.getenv("DEEPSEEK_API_KEY")
)

llm_reasoner = ChatOpenAI(
    model="deepseek-reasoner",
    temperature=0.3,
    base_url="https://api.deepseek.com/v1",
    api_key=os.getenv("DEEPSEEK_API_KEY")
)


# ============ 3. 任务管理器 ============

class TaskManager:
    """
    任务管理器

    负责任务的增删改查，以及基本的列表展示。
    """

    def __init__(self):
        self.tasks: Dict[str, Task] = {}
        self._counter = 0

    def create_task(
        self,
        description: str,
        urgency: int = 5,
        importance: int = 5,
        estimated_hours: float = 1.0,
        dependencies: Optional[List[str]] = None
    ) -> Task:
        """创建新任务"""
        self._counter += 1
        task_id = f"TASK-{self._counter:03d}"
        task = Task(
            id=task_id,
            description=description,
            urgency=urgency,
            importance=importance,
            estimated_hours=estimated_hours,
            dependencies=dependencies or []
        )
        self.tasks[task_id] = task
        return task

    def update_task(self, task_id: str, **kwargs) -> Optional[Task]:
        """更新任务属性"""
        task = self.tasks.get(task_id)
        if not task:
            return None
        for key, value in kwargs.items():
            if hasattr(task, key):
                setattr(task, key, value)
        return task

    def get_task(self, task_id: str) -> Optional[Task]:
        return self.tasks.get(task_id)

    def list_tasks(self, status: Optional[TaskStatus] = None) -> List[Task]:
        """列出任务，可选按状态过滤"""
        tasks = list(self.tasks.values())
        if status:
            tasks = [t for t in tasks if t.status == status]
        return sorted(tasks, key=lambda t: t.score(), reverse=True)

    def get_stats(self) -> Dict:
        """获取统计信息"""
        total = len(self.tasks)
        by_status = defaultdict(int)
        by_priority = defaultdict(int)
        for t in self.tasks.values():
            by_status[t.status.value] += 1
            by_priority[t.priority.value] += 1
        return {
            "total": total,
            "by_status": dict(by_status),
            "by_priority": dict(by_priority)
        }

    def print_tasks(self, title: str = "任务列表"):
        """打印格式化的任务列表"""
        print(f"\n{'─' * 70}")
        print(f"{title}")
        print(f"{'─' * 70}")
        tasks = self.list_tasks()
        if not tasks:
            print("  (无任务)")
            return
        for t in tasks:
            status_icon = {
                TaskStatus.PENDING: "⏳",
                TaskStatus.IN_PROGRESS: "🔄",
                TaskStatus.COMPLETED: "✅",
                TaskStatus.BLOCKED: "🚫"
            }.get(t.status, "❓")
            assignee = t.assigned_to or "未分配"
            deps = f" [依赖: {', '.join(t.dependencies)}]" if t.dependencies else ""
            print(f"  {status_icon} {t.id} [{t.priority.value}] "
                  f"(紧急{t.urgency}/重要{t.importance}) "
                  f"→ {assignee}{deps}")
            print(f"     {t.description[:60]}{'...' if len(t.description) > 60 else ''}")


# ============ 4. 优先级评估器 ============

class PriorityEvaluator:
    """
    优先级评估器

    使用 LLM 分析任务描述，评估 urgency 和 importance，
    并给出 PriorityLevel 建议。
    """

    EVAL_PROMPT = """你是一个项目管理专家。请分析以下任务，评估其紧急性和重要性。

评估标准：
- 紧急性 (1-10): 时间压力有多大？ deadline 是否临近？延迟是否会造成严重后果？
- 重要性 (1-10): 对项目目标的贡献有多大？是否是关键路径上的任务？

任务描述: {description}

只返回 JSON 格式（不要其他内容）：
{{"urgency": 1-10, "importance": 1-10, "priority": "P0|P1|P2", "reason": "简要说明评估理由"}}
"""

    def __init__(self):
        self.prompt = ChatPromptTemplate.from_messages([
            ("system", self.EVAL_PROMPT),
            ("human", "{description}")
        ])
        self.chain = self.prompt | llm | StrOutputParser()

    def evaluate(self, task: Task) -> Dict:
        """评估单个任务"""
        try:
            result = self.chain.invoke({"description": task.description})
            # 尝试提取 JSON
            import json as _json
            # 清理可能的 markdown 代码块
            result = result.replace("```json", "").replace("```", "").strip()
            data = _json.loads(result)

            # 更新任务属性
            task.urgency = max(1, min(10, data.get("urgency", 5)))
            task.importance = max(1, min(10, data.get("importance", 5)))
            task.priority = PriorityLevel(data.get("priority", "P1"))

            return {
                "success": True,
                "urgency": task.urgency,
                "importance": task.importance,
                "priority": task.priority.value,
                "reason": data.get("reason", "")
            }
        except Exception as e:
            # 回退：基于关键词启发式评估
            return self._heuristic_evaluate(task)

    def _heuristic_evaluate(self, task: Task) -> Dict:
        """启发式评估（不调用 LLM）"""
        desc = task.description.lower()

        # 紧急关键词
        urgent_kw = ["紧急", "urgent", "asap", "立刻", "马上", "立即", " critical", "deadline"]
        urgency = 8 if any(kw in desc for kw in urgent_kw) else 5

        # 重要关键词
        important_kw = ["核心", "关键", "重要", "critical", "essential", "security", "bug"]
        importance = 8 if any(kw in desc for kw in important_kw) else 5

        # 确定优先级
        if urgency >= 8 or importance >= 8:
            priority = PriorityLevel.P0
        elif urgency >= 5 or importance >= 5:
            priority = PriorityLevel.P1
        else:
            priority = PriorityLevel.P2

        task.urgency = urgency
        task.importance = importance
        task.priority = priority

        return {
            "success": True,
            "urgency": urgency,
            "importance": importance,
            "priority": priority.value,
            "reason": "基于关键词的启发式评估",
            "method": "heuristic"
        }


# ============ 5. 资源分配器 ============

class ResourceAllocator:
    """
    资源分配器

    根据任务优先级和工作人员的当前负载，智能分配任务。
    优先将高优先级任务分配给可用的人员。
    """

    def __init__(self, workers: List[Worker]):
        self.workers = {w.name: w for w in workers}

    def allocate(self, tasks: List[Task]) -> Dict[str, List[str]]:
        """
        分配任务给工作人员

        策略：
        1. 按优先级排序任务
        2. 优先将 P0 任务分配给负载最低的人员
        3. P1 和 P2 类似处理
        """
        allocation = defaultdict(list)

        # 按优先级和分数排序
        sorted_tasks = sorted(
            [t for t in tasks if t.status == TaskStatus.PENDING and not t.assigned_to],
            key=lambda t: (t.priority.value, t.score()),
        )

        for task in sorted_tasks:
            # 检查依赖是否完成
            if task.dependencies:
                deps_completed = all(
                    self._is_dependency_completed(dep_id, tasks)
                    for dep_id in task.dependencies
                )
                if not deps_completed:
                    task.status = TaskStatus.BLOCKED
                    continue

            # 找到最合适的工作人员
            best_worker = self._find_best_worker()
            if best_worker:
                task.assigned_to = best_worker.name
                best_worker.current_tasks.append(task.id)
                task.status = TaskStatus.IN_PROGRESS
                allocation[best_worker.name].append(task.id)

        return dict(allocation)

    def _find_best_worker(self) -> Optional[Worker]:
        """找到当前负载最低且有空闲的工作人员"""
        available = [w for w in self.workers.values() if w.is_available()]
        if not available:
            return None
        return min(available, key=lambda w: w.workload())

    def _is_dependency_completed(self, dep_id: str, all_tasks: List[Task]) -> bool:
        """检查依赖任务是否已完成"""
        for t in all_tasks:
            if t.id == dep_id:
                return t.status == TaskStatus.COMPLETED
        return False

    def print_allocation(self, allocation: Dict):
        """打印分配结果"""
        print(f"\n{'─' * 70}")
        print("资源分配结果")
        print(f"{'─' * 70}")
        for worker_name, task_ids in allocation.items():
            worker = self.workers[worker_name]
            load_bar = "█" * int(worker.workload() * 10) + "░" * (10 - int(worker.workload() * 10))
            print(f"  👤 {worker_name} [{load_bar}] {len(worker.current_tasks)}/{worker.max_capacity}")
            for tid in task_ids:
                print(f"     └─ {tid}")


# ============ 6. 动态重新排序器 ============

class DynamicReprioritizer:
    """
    动态重新排序器

    当新的紧急任务插入或环境变化时，重新评估所有任务的优先级，
    并可能重新分配资源。
    """

    def __init__(self, task_manager: TaskManager, evaluator: PriorityEvaluator, allocator: ResourceAllocator):
        self.task_manager = task_manager
        self.evaluator = evaluator
        self.allocator = allocator

    def handle_new_task(self, description: str, is_urgent: bool = False) -> Task:
        """
        处理新任务插入

        如果是紧急任务，可能会触发重新排序。
        """
        print(f"\n📥 新任务到达: {description}")
        if is_urgent:
            print("   ⚡ 标记为紧急！")

        # 创建任务
        task = self.task_manager.create_task(
            description=description,
            urgency=9 if is_urgent else 5
        )

        # 评估优先级
        result = self.evaluator.evaluate(task)
        print(f"   📊 评估结果: 紧急{task.urgency}/重要{task.importance} → {task.priority.value}")
        print(f"   📝 理由: {result.get('reason', '')}")

        # 如果是 P0，检查是否需要重新分配
        if task.priority == PriorityLevel.P0:
            print("   🔄 P0 任务触发重新排序...")
            self._reprioritize()

        return task

    def _reprioritize(self):
        """重新排序：释放一些低优先级任务的资源，优先处理 P0"""
        tasks = self.task_manager.list_tasks()

        # 找到所有进行中的 P2 任务，尝试暂停以释放资源
        for task in tasks:
            if task.status == TaskStatus.IN_PROGRESS and task.priority == PriorityLevel.P2:
                # 释放工作人员
                if task.assigned_to and task.assigned_to in self.allocator.workers:
                    worker = self.allocator.workers[task.assigned_to]
                    if task.id in worker.current_tasks:
                        worker.current_tasks.remove(task.id)
                task.status = TaskStatus.PENDING
                task.assigned_to = None
                print(f"   ⏸️ 暂停低优先级任务 {task.id} 以释放资源")

        # 重新分配
        allocation = self.allocator.allocate(tasks)
        if allocation:
            self.allocator.print_allocation(allocation)


# ============ 7. 主系统：智能项目管理助手 ============

class ProjectManagementSystem:
    """
    智能项目管理助手

    整合所有组件，实现完整的优先级排序流程：
    1. 接收任务
    2. 评估优先级
    3. 分配资源
    4. 动态重新排序
    """

    def __init__(self):
        self.task_manager = TaskManager()
        self.evaluator = PriorityEvaluator()
        workers = [
            Worker("Worker A", skills=["前端", "UI设计"], max_capacity=3),
            Worker("Worker B", skills=["后端", "数据库"], max_capacity=2),
            Worker("Worker C", skills=["测试", "文档"], max_capacity=3),
        ]
        self.allocator = ResourceAllocator(workers)
        self.reprioritizer = DynamicReprioritizer(
            self.task_manager, self.evaluator, self.allocator
        )

    def add_task(self, description: str, is_urgent: bool = False) -> Task:
        """添加任务"""
        return self.reprioritizer.handle_new_task(description, is_urgent)

    def batch_add_tasks(self, tasks_data: List[Dict]):
        """批量添加任务"""
        print(f"\n{'=' * 70}")
        print("批量添加任务")
        print(f"{'=' * 70}")
        for data in tasks_data:
            self.add_task(
                description=data["description"],
                is_urgent=data.get("urgent", False)
            )

    def run_allocation(self):
        """执行资源分配"""
        tasks = self.task_manager.list_tasks()
        allocation = self.allocator.allocate(tasks)
        self.allocator.print_allocation(allocation)

    def complete_task(self, task_id: str):
        """标记任务完成"""
        task = self.task_manager.get_task(task_id)
        if task:
            task.status = TaskStatus.COMPLETED
            if task.assigned_to and task.assigned_to in self.allocator.workers:
                worker = self.allocator.workers[task.assigned_to]
                if task.id in worker.current_tasks:
                    worker.current_tasks.remove(task.id)
            print(f"\n✅ 任务 {task_id} 已完成")
            # 尝试重新分配被阻塞的任务
            self._unblock_tasks()

    def _unblock_tasks(self):
        """尝试解除被阻塞的任务"""
        blocked = [t for t in self.task_manager.list_tasks() if t.status == TaskStatus.BLOCKED]
        if blocked:
            print(f"   检查 {len(blocked)} 个被阻塞的任务...")
            self.run_allocation()

    def print_summary(self):
        """打印项目状态摘要"""
        self.task_manager.print_tasks("当前项目状态")
        stats = self.task_manager.get_stats()
        print(f"\n{'─' * 70}")
        print("统计信息")
        print(f"{'─' * 70}")
        print(f"  总任务数: {stats['total']}")
        print(f"  按状态: {stats['by_status']}")
        print(f"  按优先级: {stats['by_priority']}")

        print(f"\n{'─' * 70}")
        print("团队负载")
        print(f"{'─' * 70}")
        for name, worker in self.allocator.workers.items():
            load_pct = worker.workload() * 100
            print(f"  {name}: {len(worker.current_tasks)}/{worker.max_capacity} 任务 ({load_pct:.0f}%)")


# ============ 8. 对比：无优先级系统 ============

class NaiveTaskSystem:
    """
    无优先级系统（用于对比）

    简单地按到达顺序处理任务，不考虑优先级或资源约束。
    """

    def __init__(self):
        self.tasks: List[Dict] = []
        self._counter = 0

    def add_task(self, description: str):
        self._counter += 1
        self.tasks.append({
            "id": f"TASK-{self._counter:03d}",
            "description": description,
            "status": "pending"
        })

    def process_all(self):
        """按顺序处理所有任务"""
        print(f"\n{'─' * 70}")
        print("无优先级系统：按到达顺序处理")
        print(f"{'─' * 70}")
        for t in self.tasks:
            print(f"  处理 {t['id']}: {t['description'][:50]}...")
            t["status"] = "completed"

    def print_tasks(self):
        print(f"\n{'─' * 70}")
        print("无优先级系统任务队列")
        print(f"{'─' * 70}")
        for t in self.tasks:
            print(f"  {t['id']}: {t['description'][:50]}...")


# ============ 9. 演示 ============

def demo_priority_system():
    """演示智能优先级排序系统"""
    print("=" * 70)
    print("Chapter 20: Prioritization (优先级排序)")
    print("=" * 70)
    print("""
本示例展示 Prioritization 模式的核心能力：
1. 自动评估任务优先级
2. 基于优先级和资源负载智能分配
3. 动态重新排序应对紧急任务
4. 依赖关系管理
    """)

    # 创建系统
    system = ProjectManagementSystem()

    # 场景1：初始任务批量导入
    print(f"\n{'=' * 70}")
    print("场景 1: 项目启动 - 批量导入任务")
    print(f"{'=' * 70}")

    initial_tasks = [
        {"description": "设计新的用户登录页面，需要支持OAuth2和手机号登录"},
        {"description": "修复生产环境的数据库连接池泄漏问题，影响用户体验"},
        {"description": "更新产品文档，添加新功能的API说明"},
        {"description": "优化首页加载速度，目标是从3秒降到1秒以内"},
        {"description": "为移动端适配新的支付流程"},
    ]
    system.batch_add_tasks(initial_tasks)
    system.run_allocation()
    system.print_summary()

    # 场景2：紧急任务插入
    print(f"\n{'=' * 70}")
    print("场景 2: 突发情况 - 紧急任务插入")
    print(f"{'=' * 70}")

    urgent_task = system.add_task(
        "紧急：生产环境出现安全漏洞，需要立即修复SQL注入问题",
        is_urgent=True
    )
    system.task_manager.print_tasks("插入紧急任务后的状态")

    # 场景3：任务完成，资源释放
    print(f"\n{'=' * 70}")
    print("场景 3: 任务完成 - 资源释放与重新分配")
    print(f"{'=' * 70}")

    # 找到最高优先级的进行中任务并标记完成
    tasks = system.task_manager.list_tasks(TaskStatus.IN_PROGRESS)
    if tasks:
        system.complete_task(tasks[0].id)
    system.print_summary()

    return system


def demo_comparison():
    """对比：有优先级 vs 无优先级"""
    print(f"\n{'=' * 70}")
    print("对比演示：有优先级 vs 无优先级")
    print(f"{'=' * 70}")

    task_descriptions = [
        "紧急修复安全漏洞",
        "更新产品文档",
        "优化首页加载速度",
        "修复数据库连接池泄漏",
    ]

    # 无优先级系统
    print("\n【方案 A】无优先级系统")
    naive = NaiveTaskSystem()
    for desc in task_descriptions:
        naive.add_task(desc)
    naive.print_tasks()
    naive.process_all()

    # 有优先级系统
    print("\n【方案 B】智能优先级系统")
    smart = ProjectManagementSystem()
    for desc in task_descriptions:
        smart.add_task(desc)
    smart.run_allocation()
    smart.print_summary()

    print(f"\n{'─' * 70}")
    print("对比分析")
    print(f"{'─' * 70}")
    print("""
【无优先级系统】
- 任务按到达顺序处理
- 紧急任务可能被排在后面
- 无法根据资源负载合理分配
- 简单但低效

【智能优先级系统】
- 自动评估紧急性和重要性
- 高优先级任务优先处理
- 考虑团队负载平衡分配
- 紧急插入时动态重新排序
- 支持依赖关系管理
""")


def main():
    # 主演示
    system = demo_priority_system()

    # 对比演示
    demo_comparison()

    # 总结
    print("\n" + "=" * 70)
    print("Prioritization 优势总结")
    print("=" * 70)
    print("""
【核心优势】

1. **目标对齐**
   - 优先处理对项目目标贡献最大的任务
   - 避免在次要任务上浪费资源
   - 确保关键路径上的任务不被阻塞

2. **资源优化**
   - 根据团队负载动态分配任务
   - 避免某些人过载、某些人空闲
   - 最大化整体吞吐量

3. **响应能力**
   - 紧急任务可以快速插入并得到处理
   - 动态重新排序适应变化的环境
   - 支持依赖关系管理

4. **可解释性**
   - 每个任务的优先级都有明确理由
   - 分配决策透明可追踪
   - 便于项目管理者审计和调整

【适用场景】

- 项目管理助手
- 客户服务工单系统
- 自动驾驶决策系统
- 云计算资源调度
- 智能制造任务调度

【与其他模式结合】

- + Planning: 规划器生成任务列表，优先级排序器决定执行顺序
- + Multi-Agent: 不同 Agent 竞争资源时，优先级决定分配
- + Resource-Aware: 在资源约束下做更精细的优先级权衡
- + Reflection: Agent 自我评估任务完成质量，反馈调整优先级策略

【实现要点】

1. **评估标准**：明确 urgency、importance、effort 等维度
2. **动态性**：环境变化时能够重新评估和调整
3. **公平性**：避免低优先级任务永远得不到处理（饥饿问题）
4. **依赖管理**：确保依赖任务先于被依赖任务完成
""")


if __name__ == "__main__":
    main()
