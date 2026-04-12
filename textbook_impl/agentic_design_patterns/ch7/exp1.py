"""
Chapter 7: Multi-Agent Pattern (多智能体模式)

多智能体模式是 Agentic Design Patterns 中的高级协作模式。
它通过多个专业智能体的协作，完成单一智能体难以处理的复杂任务。

核心思想：
- 将复杂任务分解并分配给不同专业领域的智能体
- 每个智能体专注于自己的专业领域
- 智能体之间通过消息传递进行协作
- 通过协调器管理智能体之间的交互

两种主要架构：
1. 层级式（Hierarchical）：中央协调器 + 多个专业智能体
2. 扁平式（Peer-to-Peer）：智能体之间直接协作，无主从关系

示例场景：软件开发团队协作，包括产品经理、架构师、开发工程师、测试工程师
"""

import os
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


# ============ 1. 消息系统 ============

class MessageType(Enum):
    """消息类型"""
    TASK = "task"           # 任务分配
    RESULT = "result"       # 任务结果
    QUESTION = "question"   # 询问
    ANSWER = "answer"       # 回答
    SYSTEM = "system"       # 系统消息


@dataclass
class Message:
    """智能体间传递的消息"""
    sender: str
    receiver: str
    content: str
    msg_type: MessageType = MessageType.TASK
    timestamp: Optional[str] = None


# ============ 2. 基础智能体 ============

class Agent:
    """基础智能体类"""

    def __init__(self, name: str, role: str, expertise: str):
        self.name = name
        self.role = role
        self.expertise = expertise
        self.memory: List[Message] = []  # 智能体的记忆

    def receive_message(self, message: Message):
        """接收消息"""
        self.memory.append(message)

    def process(self, task: str, context: str = "") -> str:
        """处理任务（子类需要重写）"""
        raise NotImplementedError

    def __repr__(self):
        return f"Agent({self.name}, {self.role})"


# ============ 3. 专业智能体实现 ============

class ProductManagerAgent(Agent):
    """产品经理智能体：负责需求分析和产品规划"""

    def __init__(self):
        super().__init__(
            name="产品经理",
            role="Product Manager",
            expertise="需求分析、产品规划、用户故事编写"
        )
        system_msg = """你是一位资深产品经理。你的职责是：
1. 分析用户需求，提取核心功能点
2. 编写清晰的用户故事
3. 制定产品功能优先级
4. 定义验收标准

输出格式：
- 需求概述
- 核心功能列表
- 用户故事
- 验收标准"""
        self.prompt = ChatPromptTemplate.from_messages([
            ("system", system_msg),
            ("human", "任务: {task}\n\n上下文信息:\n{context}")
        ])
        self.chain = self.prompt | llm | StrOutputParser()

    def process(self, task: str, context: str = "") -> str:
        return self.chain.invoke({"task": task, "context": context})


class ArchitectAgent(Agent):
    """架构师智能体：负责技术架构设计"""

    def __init__(self):
        super().__init__(
            name="架构师",
            role="Software Architect",
            expertise="系统设计、技术选型、架构模式"
        )
        system_msg = """你是一位资深软件架构师。你的职责是：
1. 设计系统整体架构
2. 选择合适的技术栈
3. 定义模块划分和接口
4. 考虑性能、可扩展性、安全性

输出格式：
- 架构概述
- 技术栈选择
- 模块划分
- 关键接口定义
- 架构图描述"""
        self.prompt = ChatPromptTemplate.from_messages([
            ("system", system_msg),
            ("human", "任务: {task}\n\n产品需求:\n{requirements}\n\n上下文信息:\n{context}")
        ])
        self.chain = self.prompt | llm | StrOutputParser()

    def process(self, task: str, context: str = "") -> str:
        # 从产品需求中提取
        return self.chain.invoke({
            "task": task,
            "requirements": context,
            "context": self._get_recent_context()
        })

    def _get_recent_context(self) -> str:
        """获取最近的对话上下文"""
        recent = self.memory[-3:] if len(self.memory) >= 3 else self.memory
        return "\n".join([f"{m.sender}: {m.content[:200]}..." for m in recent])


class DeveloperAgent(Agent):
    """开发工程师智能体：负责代码实现"""

    def __init__(self):
        super().__init__(
            name="开发工程师",
            role="Developer",
            expertise="代码实现、功能开发、单元测试"
        )
        system_msg = """你是一位资深开发工程师。你的职责是：
1. 根据架构设计实现功能代码
2. 编写清晰、可维护的代码
3. 实现单元测试
4. 确保代码符合最佳实践

输出格式：
- 实现说明
- 核心代码（Python）
- 单元测试代码
- 关键算法解释"""
        self.prompt = ChatPromptTemplate.from_messages([
            ("system", system_msg),
            ("human", "任务: {task}\n\n架构设计:\n{architecture}\n\n上下文信息:\n{context}")
        ])
        self.chain = self.prompt | llm | StrOutputParser()

    def process(self, task: str, context: str = "") -> str:
        return self.chain.invoke({
            "task": task,
            "architecture": context,
            "context": self._get_recent_context()
        })

    def _get_recent_context(self) -> str:
        recent = self.memory[-3:] if len(self.memory) >= 3 else self.memory
        return "\n".join([f"{m.sender}: {m.content[:200]}..." for m in recent])


class TesterAgent(Agent):
    """测试工程师智能体：负责测试设计"""

    def __init__(self):
        super().__init__(
            name="测试工程师",
            role="QA Engineer",
            expertise="测试设计、测试用例编写、缺陷分析"
        )
        system_msg = """你是一位资深测试工程师。你的职责是：
1. 根据需求设计测试用例
2. 编写测试计划
3. 识别潜在风险点
4. 提出质量保障建议

输出格式：
- 测试策略
- 测试用例列表
- 边界情况分析
- 质量风险评估"""
        self.prompt = ChatPromptTemplate.from_messages([
            ("system", system_msg),
            ("human", "任务: {task}\n\n需求文档:\n{requirements}\n\n实现代码:\n{code}\n\n上下文信息:\n{context}")
        ])
        self.chain = self.prompt | llm | StrOutputParser()

    def process(self, task: str, context: str = "") -> str:
        # 解析 context 中的需求和代码
        parts = context.split("\n===CODE===\n")
        requirements = parts[0] if len(parts) > 0 else ""
        code = parts[1] if len(parts) > 1 else ""

        return self.chain.invoke({
            "task": task,
            "requirements": requirements,
            "code": code,
            "context": self._get_recent_context()
        })

    def _get_recent_context(self) -> str:
        recent = self.memory[-3:] if len(self.memory) >= 3 else self.memory
        return "\n".join([f"{m.sender}: {m.content[:200]}..." for m in recent])


# ============ 4. 协调器 ============

class Coordinator:
    """
    中央协调器
    负责管理智能体之间的协作流程
    """

    def __init__(self):
        self.agents: Dict[str, Agent] = {}
        self.message_bus: List[Message] = []

    def register_agent(self, agent: Agent):
        """注册智能体"""
        self.agents[agent.name] = agent

    def send_message(self, message: Message):
        """发送消息"""
        self.message_bus.append(message)
        if message.receiver in self.agents:
            self.agents[message.receiver].receive_message(message)

    def broadcast(self, sender: str, content: str, msg_type: MessageType = MessageType.SYSTEM):
        """广播消息给所有智能体"""
        for name in self.agents:
            if name != sender:
                self.send_message(Message(
                    sender=sender,
                    receiver=name,
                    content=content,
                    msg_type=msg_type
                ))

    def get_agent(self, name: str) -> Optional[Agent]:
        """获取智能体"""
        return self.agents.get(name)


# ============ 5. 多智能体系统 ============

class MultiAgentSystem:
    """
    多智能体协作系统
    采用层级式架构，中央协调器协调多个专业智能体
    """

    def __init__(self):
        self.coordinator = Coordinator()
        self._init_agents()

    def _init_agents(self):
        """初始化所有专业智能体"""
        self.coordinator.register_agent(ProductManagerAgent())
        self.coordinator.register_agent(ArchitectAgent())
        self.coordinator.register_agent(DeveloperAgent())
        self.coordinator.register_agent(TesterAgent())

    def execute_project(self, project_requirement: str, verbose: bool = True) -> Dict:
        """
        执行项目开发流程

        流程：
        1. 产品经理分析需求
        2. 架构师设计架构
        3. 开发工程师实现代码
        4. 测试工程师设计测试
        5. 协调器整合结果
        """
        if verbose:
            print("=" * 80)
            print("Multi-Agent Pattern 示例 - 软件开发团队协作")
            print("=" * 80)
            print(f"\n项目需求: {project_requirement}\n")

        results = {}

        # 阶段 1: 需求分析
        if verbose:
            print("─" * 80)
            print("阶段 1: 需求分析 (产品经理)")
            print("─" * 80)

        pm = self.coordinator.get_agent("产品经理")
        requirements = pm.process(project_requirement)
        results["requirements"] = requirements

        self.coordinator.broadcast(
            "产品经理",
            f"需求分析完成: {requirements[:300]}...",
            MessageType.RESULT
        )

        if verbose:
            print(f"\n📋 需求分析结果:\n{requirements}\n")

        # 阶段 2: 架构设计
        if verbose:
            print("─" * 80)
            print("阶段 2: 架构设计 (架构师)")
            print("─" * 80)

        architect = self.coordinator.get_agent("架构师")
        architecture = architect.process("设计系统架构", requirements)
        results["architecture"] = architecture

        self.coordinator.broadcast(
            "架构师",
            f"架构设计完成: {architecture[:300]}...",
            MessageType.RESULT
        )

        if verbose:
            print(f"\n🏗️ 架构设计结果:\n{architecture}\n")

        # 阶段 3: 代码实现
        if verbose:
            print("─" * 80)
            print("阶段 3: 代码实现 (开发工程师)")
            print("─" * 80)

        developer = self.coordinator.get_agent("开发工程师")
        code = developer.process("实现核心功能", architecture)
        results["code"] = code

        self.coordinator.broadcast(
            "开发工程师",
            f"代码实现完成: {code[:300]}...",
            MessageType.RESULT
        )

        if verbose:
            print(f"\n💻 代码实现结果:\n{code}\n")

        # 阶段 4: 测试设计
        if verbose:
            print("─" * 80)
            print("阶段 4: 测试设计 (测试工程师)")
            print("─" * 80)

        tester = self.coordinator.get_agent("测试工程师")
        test_context = f"{requirements}\n===CODE===\n{code}"
        test_plan = tester.process("设计测试方案", test_context)
        results["test_plan"] = test_plan

        if verbose:
            print(f"\n🧪 测试设计结果:\n{test_plan}\n")

        # 阶段 5: 项目总结
        if verbose:
            print("─" * 80)
            print("阶段 5: 项目总结")
            print("─" * 80)

        summary = self._generate_summary(results)
        results["summary"] = summary

        if verbose:
            print(f"\n📊 项目总结:\n{summary}\n")

        return results

    def _generate_summary(self, results: Dict) -> str:
        """生成项目总结"""
        system_msg = """你是一位项目总监。请根据各阶段的输出，生成项目总结报告。

总结应包括：
1. 项目概述
2. 各阶段成果摘要
3. 关键决策点
4. 风险与建议
5. 下一步行动计划"""

        human_msg = """需求分析:
{requirements}

架构设计:
{architecture}

代码实现:
{code}

测试方案:
{test_plan}

请生成项目总结报告："""

        prompt = ChatPromptTemplate.from_messages([
            ("system", system_msg),
            ("human", human_msg)
        ])
        chain = prompt | llm | StrOutputParser()

        return chain.invoke({
            "requirements": results["requirements"][:500],
            "architecture": results["architecture"][:500],
            "code": results["code"][:500],
            "test_plan": results["test_plan"][:500]
        })


# ============ 6. 对比：单智能体 vs 多智能体 ============

class SingleAgentSystem:
    """单智能体系统（用于对比）"""

    def __init__(self):
        system_msg = """你是一位全栈软件工程师，同时具备产品经理、架构师、开发工程师和测试工程师的能力。
请独立完成从需求分析到测试设计的全部工作。"""
        self.prompt = ChatPromptTemplate.from_messages([
            ("system", system_msg),
            ("human", "项目需求: {requirement}")
        ])
        self.chain = self.prompt | llm | StrOutputParser()

    def execute(self, requirement: str) -> str:
        return self.chain.invoke({"requirement": requirement})


# ============ 7. 演示 ============

def main():
    # 测试项目需求
    project_requirement = """
开发一个任务管理系统的核心模块，需要支持：
1. 创建、编辑、删除任务
2. 任务状态管理（待办、进行中、已完成）
3. 任务优先级设置
4. 任务截止日期提醒
5. 简单的任务筛选功能

技术约束：
- 使用 Python 实现
- 需要支持并发访问
- 数据需要持久化存储
"""

    print("=" * 80)
    print("Multi-Agent Pattern 示例 - 软件开发团队协作")
    print("=" * 80)

    # 多智能体协作模式
    print("\n" + "=" * 80)
    print("模式一：多智能体协作（专业分工）")
    print("=" * 80)

    multi_agent = MultiAgentSystem()
    multi_results = multi_agent.execute_project(project_requirement, verbose=True)

    # 单智能体模式（对比）
    print("\n" + "=" * 80)
    print("模式二：单智能体（一人全包）- 用于对比")
    print("=" * 80)

    single_agent = SingleAgentSystem()
    print("\n单智能体执行结果（前1500字符）：\n")
    single_result = single_agent.execute(project_requirement)
    print(single_result[:1500] + "...")

    # 总结
    print("\n" + "=" * 80)
    print("Multi-Agent Pattern 优势总结")
    print("=" * 80)
    print("""
【核心优势】

1. **专业分工**
   - 每个智能体专注于特定领域
   - 发挥各自专业优势
   - 输出质量更高

2. **可扩展性**
   - 易于添加新的专业智能体
   - 可以组合不同能力
   - 支持复杂工作流

3. **可追溯性**
   - 每个阶段有明确的负责人
   - 中间产物清晰可见
   - 便于问题定位

4. **并行潜力**
   - 无依赖的任务可并行执行
   - 提高整体效率
   - 缩短项目周期

【两种架构对比】

| 架构 | 优点 | 缺点 | 适用场景 |
|------|------|------|----------|
| 层级式 | 结构清晰、易于管理 | 单点瓶颈 | 任务流程明确 |
| 扁平式 | 灵活、去中心化 | 协调复杂 | 探索性任务 |

【适用场景】

- 软件开发团队协作
- 复杂数据分析项目
- 多领域研究任务
- 内容创作团队
- 客户服务系统
- 智能运维平台

【与其他模式结合】

- + Planning: 协调器制定协作计划
- + Reflection: 每个智能体自我反思改进
- + Tool Use: 智能体调用专业工具
- + Routing: 任务路由到最合适的智能体

【实现要点】

1. **消息系统**：设计清晰的消息传递机制
2. **角色定义**：明确每个智能体的职责边界
3. **协调策略**：决定协作流程和冲突解决
4. **记忆管理**：维护智能体的上下文记忆
5. **错误处理**：处理智能体失败和超时情况
""")


if __name__ == "__main__":
    main()
