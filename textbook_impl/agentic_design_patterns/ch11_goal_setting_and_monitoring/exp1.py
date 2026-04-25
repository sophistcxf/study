"""
Chapter 11: Goal Setting and Monitoring Pattern (目标设定与监控模式)

目标设定与监控模式是 Agentic Design Patterns 中的重要模式。
它让智能体能够设定明确的目标，并持续监控进展，根据反馈进行调整。

核心思想：
- 将大目标分解为可衡量的小目标（子目标）
- 建立明确的评估标准和进度指标
- 持续监控执行状态
- 根据监控结果动态调整策略
- 确保最终目标的达成

示例场景：学习规划助手，帮助学生设定学习目标并跟踪进度

本示例展示了：
1. 目标分解：将大目标拆分为具体的子目标
2. 进度跟踪：实时监控每个子目标的完成情况
3. 动态调整：根据进度情况调整学习计划
4. 反馈循环：定期评估并提供改进建议
"""

import os
import json
from typing import Dict, List, Optional
from dataclasses import dataclass, field, asdict
from datetime import datetime, timedelta
from enum import Enum
from langchain_openai import ChatOpenAI
from langchain_core.prompts import ChatPromptTemplate
from langchain_core.output_parsers import StrOutputParser, JsonOutputParser
from dotenv import load_dotenv

load_dotenv()

# 初始化 LLM
llm = ChatOpenAI(
    model="deepseek-chat",
    temperature=0,
    base_url="https://api.deepseek.com/v1",
    api_key=os.getenv("DEEPSEEK_API_KEY")
)


# ============ 1. 数据模型定义 ============

class GoalStatus(Enum):
    """目标状态枚举"""
    NOT_STARTED = "未开始"
    IN_PROGRESS = "进行中"
    COMPLETED = "已完成"
    BLOCKED = "受阻"
    ADJUSTED = "已调整"


@dataclass
class SubGoal:
    """子目标"""
    id: str
    description: str
    status: GoalStatus = GoalStatus.NOT_STARTED
    progress: float = 0.0  # 0-100
    deadline: Optional[str] = None
    metrics: List[str] = field(default_factory=list)  # 衡量指标
    notes: List[str] = field(default_factory=list)  # 备注/反馈


@dataclass
class MainGoal:
    """主目标"""
    id: str
    title: str
    description: str
    deadline: str
    status: GoalStatus = GoalStatus.NOT_STARTED
    overall_progress: float = 0.0
    sub_goals: List[SubGoal] = field(default_factory=list)
    created_at: str = field(default_factory=lambda: datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
    last_updated: str = field(default_factory=lambda: datetime.now().strftime("%Y-%m-%d %H:%M:%S"))


# ============ 2. 目标设定功能 ============

def decompose_goal(main_goal_description: str, domain: str = "学习") -> List[Dict]:
    """
    使用 LLM 将大目标分解为多个可衡量的子目标
    
    Args:
        main_goal_description: 主目标描述
        domain: 领域（如：学习、工作、健身等）
    
    Returns:
        子目标列表，每个子目标包含描述、截止日期、衡量指标等
    """
    prompt = ChatPromptTemplate.from_messages([
        ("system", f"你是一个专业的{domain}规划师，擅长将大目标分解为可执行的子目标。"),
        ("human", """请分析以下目标，并将其分解为3-5个具体的子目标。

主目标：{goal_description}

对于每个子目标，请提供：
1. description: 子目标的详细描述
2. deadline_days: 完成该子目标所需的天数
3. metrics: 2-3个关键衡量指标（用逗号分隔）

请以JSON数组格式返回，例如：
[
  {{
    "description": "学习Python基础语法",
    "deadline_days": 7,
    "metrics": ["完成基础语法练习", "能够编写简单程序"]
  }}
]

只返回JSON数组，不要有其他内容。""")
    ])

    chain = prompt | llm | JsonOutputParser()
    result = chain.invoke({"goal_description": main_goal_description})
    
    # 如果返回的是字典而不是列表，尝试提取 sub_goals
    if isinstance(result, dict):
        return result.get("sub_goals", [])
    elif isinstance(result, list):
        return result
    else:
        return []


def create_goal_plan(goal_title: str, goal_description: str, days: int = 30) -> MainGoal:
    """
    创建完整的目标计划
    
    Args:
        goal_title: 目标标题
        goal_description: 目标详细描述
        days: 总天数
    
    Returns:
        MainGoal 对象
    """
    print(f"\n{'='*60}")
    print(f"📋 正在为目标 '{goal_title}' 制定计划...")
    print(f"{'='*60}\n")
    
    # 步骤1：分解目标
    sub_goals_data = decompose_goal(goal_description)
    
    # 步骤2：创建子目标对象
    sub_goals = []
    base_date = datetime.now()
    
    for i, sg_data in enumerate(sub_goals_data):
        deadline = base_date + timedelta(days=sg_data.get("deadline_days", 7))
        sub_goal = SubGoal(
            id=f"SG-{i+1:02d}",
            description=sg_data["description"],
            deadline=deadline.strftime("%Y-%m-%d"),
            metrics=sg_data.get("metrics", [])
        )
        sub_goals.append(sub_goal)
        print(f"✅ 子目标 {i+1}: {sub_goal.description}")
        print(f"   📅 截止日期: {sub_goal.deadline}")
        print(f"   📊 衡量指标: {', '.join(sub_goal.metrics)}\n")
    
    # 步骤3：创建主目标
    main_deadline = base_date + timedelta(days=days)
    main_goal = MainGoal(
        id=f"MG-{datetime.now().strftime('%Y%m%d%H%M%S')}",
        title=goal_title,
        description=goal_description,
        deadline=main_deadline.strftime("%Y-%m-%d"),
        sub_goals=sub_goals
    )
    
    print(f"🎯 主目标: {main_goal.title}")
    print(f"📅 总截止日期: {main_goal.deadline}")
    print(f"📝 共分解为 {len(sub_goals)} 个子目标")
    
    return main_goal


# ============ 3. 进度监控功能 ============

def update_subgoal_progress(subgoal: SubGoal, new_progress: float, feedback: str = "") -> SubGoal:
    """
    更新子目标的进度
    
    Args:
        subgoal: 子目标对象
        new_progress: 新进度（0-100）
        feedback: 反馈意见
    
    Returns:
        更新后的子目标对象
    """
    subgoal.progress = max(0, min(100, new_progress))
    
    # 根据进度更新状态
    if subgoal.progress >= 100:
        subgoal.status = GoalStatus.COMPLETED
    elif subgoal.progress > 0:
        subgoal.status = GoalStatus.IN_PROGRESS
    
    # 添加反馈记录
    if feedback:
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M")
        subgoal.notes.append(f"[{timestamp}] {feedback}")
    
    return subgoal


def calculate_overall_progress(main_goal: MainGoal) -> float:
    """
    计算主目标的总体进度（基于子目标进度的加权平均）
    
    Args:
        main_goal: 主目标对象
    
    Returns:
        总体进度百分比（0-100）
    """
    if not main_goal.sub_goals:
        return 0.0
    
    total_progress = sum(sg.progress for sg in main_goal.sub_goals)
    overall = total_progress / len(main_goal.sub_goals)
    
    # 更新主目标状态
    main_goal.overall_progress = round(overall, 2)
    
    if overall >= 100:
        main_goal.status = GoalStatus.COMPLETED
    elif overall > 0:
        main_goal.status = GoalStatus.IN_PROGRESS
    
    main_goal.last_updated = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    
    return overall


def monitor_goal_status(main_goal: MainGoal) -> Dict:
    """
    监控目标状态，生成进度报告
    
    Args:
        main_goal: 主目标对象
    
    Returns:
        包含进度报告的字典
    """
    # 计算总体进度
    overall_progress = calculate_overall_progress(main_goal)
    
    # 统计各状态的子目标数量
    status_count = {}
    for sg in main_goal.sub_goals:
        status = sg.status.value
        status_count[status] = status_count.get(status, 0) + 1
    
    # 识别需要关注的子目标
    attention_needed = [
        sg for sg in main_goal.sub_goals 
        if sg.status == GoalStatus.BLOCKED or 
        (sg.status == GoalStatus.IN_PROGRESS and sg.progress < 30)
    ]
    
    # 生成报告
    report = {
        "goal_id": main_goal.id,
        "goal_title": main_goal.title,
        "overall_progress": overall_progress,
        "status": main_goal.status.value,
        "deadline": main_goal.deadline,
        "sub_goals_summary": {
            "total": len(main_goal.sub_goals),
            "by_status": status_count
        },
        "attention_needed": [
            {
                "id": sg.id,
                "description": sg.description,
                "progress": sg.progress,
                "status": sg.status.value
            }
            for sg in attention_needed
        ],
        "generated_at": datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    }
    
    return report


def print_progress_report(report: Dict):
    """
    打印格式化的进度报告
    
    Args:
        report: 进度报告字典
    """
    print(f"\n{'='*60}")
    print(f"📊 目标进度报告")
    print(f"{'='*60}")
    print(f"🎯 目标: {report['goal_title']}")
    print(f"📈 总体进度: {report['overall_progress']:.1f}%")
    print(f"📌 状态: {report['status']}")
    print(f"📅 截止日期: {report['deadline']}")
    print(f"\n📋 子目标概览:")
    print(f"   总数: {report['sub_goals_summary']['total']}")
    for status, count in report['sub_goals_summary']['by_status'].items():
        print(f"   - {status}: {count}")
    
    if report['attention_needed']:
        print(f"\n⚠️  需要关注:")
        for item in report['attention_needed']:
            print(f"   • [{item['id']}] {item['description']}")
            print(f"     进度: {item['progress']}%, 状态: {item['status']}")
    
    print(f"\n🕐 报告生成时间: {report['generated_at']}")
    print(f"{'='*60}\n")


# ============ 4. 动态调整功能 ============

def analyze_and_adjust(main_goal: MainGoal) -> List[Dict]:
    """
    分析当前进度并提供调整建议
    
    Args:
        main_goal: 主目标对象
    
    Returns:
        调整建议列表
    """
    # 准备当前状态信息
    current_state = {
        "title": main_goal.title,
        "description": main_goal.description,
        "deadline": main_goal.deadline,
        "overall_progress": main_goal.overall_progress,
        "sub_goals": [
            {
                "id": sg.id,
                "description": sg.description,
                "progress": sg.progress,
                "status": sg.status.value,
                "deadline": sg.deadline,
                "notes": sg.notes[-3:] if sg.notes else []  # 最近3条备注
            }
            for sg in main_goal.sub_goals
        ]
    }
    
    prompt = ChatPromptTemplate.from_messages([
        ("system", "你是一个目标管理专家，擅长分析目标执行情况并提供调整建议。"),
        ("human", """请分析当前的目标执行状态，并提供具体的调整建议。

当前目标状态：
{current_state}

请以 JSON 格式返回建议，格式如下：
{{
    "analysis": "整体情况分析",
    "suggestions": [
        {{
            "sub_goal_id": "子目标ID（如果针对特定子目标）",
            "type": "优先级调整|资源重新分配|时间延长|策略改变",
            "recommendation": "具体建议内容",
            "reason": "建议原因"
        }}
    ],
    "priority_focus": ["需要优先关注的子目标ID列表"]
}}

只返回JSON，不要有其他内容。""")
    ])

    chain = prompt | llm | JsonOutputParser()
    result = chain.invoke({"current_state": json.dumps(current_state, ensure_ascii=False)})
    
    return result.get("suggestions", [])


def apply_adjustments(main_goal: MainGoal, suggestions: List[Dict]):
    """
    应用调整建议到目标计划
    
    Args:
        main_goal: 主目标对象
        suggestions: 调整建议列表
    """
    print(f"\n{'='*60}")
    print(f"🔄 应用调整建议")
    print(f"{'='*60}\n")
    
    for i, suggestion in enumerate(suggestions, 1):
        print(f"💡 建议 {i}:")
        print(f"   类型: {suggestion.get('type', 'N/A')}")
        print(f"   内容: {suggestion.get('recommendation', 'N/A')}")
        print(f"   原因: {suggestion.get('reason', 'N/A')}\n")
        
        # 如果建议针对特定子目标，添加备注
        sub_goal_id = suggestion.get("sub_goal_id")
        if sub_goal_id:
            for sg in main_goal.sub_goals:
                if sg.id == sub_goal_id:
                    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M")
                    note = f"[{timestamp}] 调整建议: {suggestion.get('recommendation', '')}"
                    sg.notes.append(note)
                    sg.status = GoalStatus.ADJUSTED
                    break
    
    main_goal.last_updated = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    print(f"✅ 已应用 {len(suggestions)} 条调整建议")


# ============ 5. 完整的工作流示例 ============

def run_goal_setting_example():
    """
    运行完整的目标设定与监控示例
    """
    print("\n" + "="*60)
    print("🚀 Chapter 11: Goal Setting and Monitoring Pattern")
    print("="*60)
    
    # 场景：学生想要学习 Python 编程
    goal_title = "掌握 Python 编程基础"
    goal_description = """
    在30天内掌握Python编程基础，包括：
    - 基本语法和数据结构
    - 函数和模块
    - 面向对象编程
    - 文件操作和异常处理
    - 能够独立完成小型项目
    """
    
    # 步骤1：创建目标计划
    print("\n【步骤1】创建目标计划")
    main_goal = create_goal_plan(goal_title, goal_description, days=30)
    
    # 步骤2：模拟进度更新
    print("\n【步骤2】模拟进度更新")
    
    # 更新第一个子目标进度
    if main_goal.sub_goals:
        sg1 = main_goal.sub_goals[0]
        sg1 = update_subgoal_progress(
            sg1, 
            new_progress=80, 
            feedback="已完成理论学习，正在做练习题"
        )
        main_goal.sub_goals[0] = sg1
        print(f"✅ 更新 {sg1.id}: 进度 80%")
        
        # 更新第二个子目标进度
        if len(main_goal.sub_goals) > 1:
            sg2 = main_goal.sub_goals[1]
            sg2 = update_subgoal_progress(
                sg2,
                new_progress=45,
                feedback="遇到一些困难，需要更多练习"
            )
            main_goal.sub_goals[1] = sg2
            print(f"✅ 更新 {sg2.id}: 进度 45%")
    
    # 步骤3：生成进度报告
    print("\n【步骤3】生成进度报告")
    report = monitor_goal_status(main_goal)
    print_progress_report(report)
    
    # 步骤4：分析并提供调整建议
    print("\n【步骤4】分析并提供调整建议")
    suggestions = analyze_and_adjust(main_goal)
    apply_adjustments(main_goal, suggestions)
    
    # 步骤5：再次生成报告
    print("\n【步骤5】调整后的最终报告")
    final_report = monitor_goal_status(main_goal)
    print_progress_report(final_report)
    
    # 步骤6：保存目标计划
    print("\n【步骤6】保存目标计划")
    save_goal_to_file(main_goal, "goal_plan.json")
    
    print("\n✨ 目标设定与监控示例完成！")
    print("="*60 + "\n")


def save_goal_to_file(main_goal: MainGoal, filename: str = "goal_plan.json"):
    """
    将目标计划保存到 JSON 文件
    
    Args:
        main_goal: 主目标对象
        filename: 文件名
    """
    # 转换为可序列化的字典
    goal_dict = asdict(main_goal)
    
    # 处理枚举类型的序列化
    def convert_enum(obj):
        if isinstance(obj, GoalStatus):
            return obj.value
        raise TypeError(f"Object of type {type(obj)} is not JSON serializable")
    
    with open(filename, 'w', encoding='utf-8') as f:
        json.dump(goal_dict, f, ensure_ascii=False, indent=2, default=convert_enum)
    
    print(f"💾 目标计划已保存到: {filename}")


def load_goal_from_file(filename: str = "goal_plan.json") -> MainGoal:
    """
    从 JSON 文件加载目标计划
    
    Args:
        filename: 文件名
    
    Returns:
        MainGoal 对象
    """
    with open(filename, 'r', encoding='utf-8') as f:
        goal_dict = json.load(f)
    
    # 恢复枚举类型
    def restore_enum(d):
        if isinstance(d, dict):
            if 'status' in d and d['status'] in [s.value for s in GoalStatus]:
                d['status'] = GoalStatus(d['status'])
            for key, value in d.items():
                d[key] = restore_enum(value)
        elif isinstance(d, list):
            d = [restore_enum(item) for item in d]
        return d
    
    goal_dict = restore_enum(goal_dict)
    
    # 重建子目标
    sub_goals = [SubGoal(**sg) for sg in goal_dict.pop('sub_goals', [])]
    main_goal = MainGoal(**goal_dict, sub_goals=sub_goals)
    
    return main_goal


# ============ 主程序入口 ============

if __name__ == "__main__":
    run_goal_setting_example()
