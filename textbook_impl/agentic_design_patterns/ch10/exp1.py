"""
Chapter 10: MCP (Model Context Protocol) 集成示例

MCP 是由 Anthropic 提出的开放协议，用于标准化 AI 模型与外部工具/数据源的连接方式。
它解决了"每个 AI 应用都要自己写工具集成"的问题，提供了一种通用的客户端-服务器架构。

核心概念：
- **MCP Server**: 暴露工具(Tools)、资源(Resources)和提示(Prompts)的服务端
- **MCP Client**: 连接到 MCP Server，发现并调用其提供的能力
- **传输协议**: stdio（标准输入输出）、SSE、Streamable HTTP
- **工具发现**: 客户端可以动态发现服务器提供的所有工具，无需硬编码

架构概览：
┌─────────────────────┐     MCP Protocol     ┌──────────────────┐
│  Host Application   │ ◄──────────────────► │   MCP Server     │
│  ┌───────────────┐  │     (stdio/HTTP)      │  ┌────────────┐ │
│  │  MCP Client   │──┼──────────────────────►│  │  Tools     │ │
│  └───────────────┘  │                       │  │  Resources │ │
│  ┌───────────────┐  │                       │  │  Prompts   │ │
│  │  LLM / Agent  │  │                       │  └────────────┘ │
│  └───────────────┘  │                       └──────────────────┘
└─────────────────────┘

与传统工具集成的对比：
| 特性       | 传统方式                | MCP 方式               |
|-----------|------------------------|------------------------|
| 工具注册   | 每个应用硬编码           | 服务器动态暴露           |
| 可复用性   | 工具与应用紧耦合         | 工具可被任何 MCP 客户端使用 |
| 发现机制   | 无，需手动配置           | 自动发现服务器提供的工具    |
| 传输协议   | 自定义                  | 标准化 (stdio/SSE/HTTP)  |
| 生态系统   | 碎片化                  | 统一标准，可互操作         |

本示例实现：
1. 创建 MCP Server（暴露天气、计算、时间、知识搜索等工具）
2. 使用 MCP Client 连接并发现工具
3. 将 MCP 工具集成到 LangChain Agent 中
4. 演示多轮对话中的工具协作
"""

import os
import sys
import json
import asyncio
from typing import List, Dict, Any

from langchain_openai import ChatOpenAI
from langchain_core.messages import (
    HumanMessage, AIMessage, ToolMessage, SystemMessage, BaseMessage
)
from dotenv import load_dotenv

# MCP imports
from mcp import ClientSession, StdioServerParameters
from mcp.client.stdio import stdio_client
from langchain_mcp_adapters.tools import load_mcp_tools

load_dotenv()


# ============ 1. 配置 ============

# MCP Server 脚本路径
SERVER_SCRIPT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "mcp_server.py")

# MCP Server 启动参数（通过 stdio 传输）
SERVER_PARAMS = StdioServerParameters(
    command=sys.executable,  # 使用当前 Python 解释器
    args=[SERVER_SCRIPT],
)

# 初始化 LLM
llm = ChatOpenAI(
    model="deepseek-chat",
    temperature=0,
    base_url="https://api.deepseek.com/v1",
    api_key=os.getenv("DEEPSEEK_API_KEY")
)


# ============ 2. MCP 基础：连接、发现、调用 ============

async def demo_mcp_basics():
    """
    演示 MCP 基础功能

    展示：
    1. 通过 stdio 建立 MCP 连接
    2. 动态发现服务器提供的工具
    3. 直接调用 MCP 工具
    """
    print("=" * 60)
    print("演示 1: MCP 基础 — 连接、发现、调用")
    print("=" * 60)

    # 通过 stdio 启动并连接 MCP Server
    async with stdio_client(SERVER_PARAMS) as (read, write):
        async with ClientSession(read, write) as session:
            # 初始化 MCP 会话（握手协议）
            await session.initialize()
            print("\n[连接] MCP 会话已建立 (stdio 传输)")

            # --- 工具发现 ---
            print("\n--- 工具发现 (list_tools) ---")
            tools_result = await session.list_tools()
            print(f"发现 {len(tools_result.tools)} 个工具:")
            for tool in tools_result.tools:
                desc = tool.description.split("\n")[0] if tool.description else ""
                print(f"  - {tool.name}: {desc}")

            # --- 直接调用工具 ---
            print("\n--- 直接调用工具 (call_tool) ---")

            # 1. 查询天气
            print("\n[调用] get_weather('北京')")
            result = await session.call_tool("get_weather", arguments={"city": "北京"})
            data = json.loads(result.content[0].text)
            print(f"  → {data.get('city')} | {data.get('condition')} | "
                  f"{data.get('temperature')} | 湿度 {data.get('humidity')}")

            # 2. 数学计算
            print("\n[调用] calculate('sqrt(144) + 2 * 3')")
            result = await session.call_tool(
                "calculate", arguments={"expression": "sqrt(144) + 2 * 3"}
            )
            data = json.loads(result.content[0].text)
            print(f"  → {data.get('expression')} = {data.get('result')}")

            # 3. 获取时间
            print("\n[调用] get_current_time()")
            result = await session.call_tool("get_current_time", arguments={})
            data = json.loads(result.content[0].text)
            print(f"  → {data.get('datetime')} ({data.get('weekday')})")

            # 4. 知识搜索
            print("\n[调用] search_knowledge('MCP', topic='tech')")
            result = await session.call_tool(
                "search_knowledge", arguments={"query": "MCP", "topic": "tech"}
            )
            data = json.loads(result.content[0].text)
            for item in data.get("results", []):
                print(f"  → [{item['topic']}] {item['content'][:80]}...")

            # 5. 单位换算
            print("\n[调用] unit_convert(100, 'km/h', 'm/s')")
            result = await session.call_tool(
                "unit_convert",
                arguments={"value": 100, "from_unit": "km/h", "to_unit": "m/s"}
            )
            data = json.loads(result.content[0].text)
            print(f"  → {data.get('original')} = {data.get('converted')}")


# ============ 3. LangChain + MCP 集成 ============

async def run_agent_loop(
    llm_with_tools,
    tool_map: Dict,
    messages: List[BaseMessage],
    max_iterations: int = 5,
    verbose: bool = True,
) -> str:
    """
    通用的 Agent 工具调用循环

    流程：LLM 推理 → 工具调用 → 结果回传 → LLM 继续推理 → ...

    Args:
        llm_with_tools: 绑定了工具的 LLM
        tool_map: 工具名 → LangChain Tool 的映射
        messages: 对话消息列表（会被原地修改）
        max_iterations: 最大工具调用轮数
        verbose: 是否打印过程

    Returns:
        LLM 的最终文本回答
    """
    response = await llm_with_tools.ainvoke(messages)
    messages.append(response)

    iteration = 0
    while response.tool_calls and iteration < max_iterations:
        iteration += 1

        if verbose:
            print(f"\n  [工具调用 - 第 {iteration} 轮]")

        for tool_call in response.tool_calls:
            tool_name = tool_call["name"]
            tool_args = tool_call["args"]

            if verbose:
                print(f"    → {tool_name}({json.dumps(tool_args, ensure_ascii=False)})")

            # 执行工具
            tool = tool_map.get(tool_name)
            if tool:
                result = await tool.ainvoke(tool_call["args"])
            else:
                result = f"未知工具: {tool_name}"

            if verbose:
                print(f"    ← {str(result)[:120]}{'...' if len(str(result)) > 120 else ''}")

            messages.append(ToolMessage(
                content=str(result),
                tool_call_id=tool_call["id"]
            ))

        # 再次调用 LLM
        response = await llm_with_tools.ainvoke(messages)
        messages.append(response)

    return response.content


async def demo_langchain_integration():
    """
    演示将 MCP 工具集成到 LangChain

    展示：
    1. 使用 load_mcp_tools 将 MCP 工具转为 LangChain 工具
    2. 通过 bind_tools 绑定到 ChatOpenAI
    3. 实现完整的 Agent 工具调用循环
    """
    print("\n" + "=" * 60)
    print("演示 2: LangChain + MCP 工具集成")
    print("=" * 60)

    async with stdio_client(SERVER_PARAMS) as (read, write):
        async with ClientSession(read, write) as session:
            await session.initialize()

            # 将 MCP 工具转换为 LangChain 工具
            tools = await load_mcp_tools(session)
            print(f"\n[加载] {len(tools)} 个 MCP 工具 → LangChain 工具:")
            for tool in tools:
                desc = tool.description.split("\n")[0] if tool.description else ""
                print(f"  - {tool.name}: {desc}")

            # 绑定工具到 LLM
            llm_with_tools = llm.bind_tools(tools)
            tool_map = {tool.name: tool for tool in tools}

            # 测试用例
            test_queries = [
                "北京和上海今天天气怎么样？哪个更热？",
                "帮我算一下 (15 * 8 + sqrt(256)) / 4 的结果，然后把结果从 celsius 转成 fahrenheit",
                "现在几点了？顺便告诉我什么是 MCP 协议",
            ]

            for query in test_queries:
                print(f"\n{'─' * 60}")
                print(f"  用户: {query}")

                messages: List[BaseMessage] = [
                    SystemMessage(content=(
                        "你是一个有用的助手，可以使用各种工具来回答问题。"
                        "请用中文回答，回答简洁明了。"
                    )),
                    HumanMessage(content=query),
                ]

                answer = await run_agent_loop(llm_with_tools, tool_map, messages)
                print(f"\n  助手: {answer[:300]}{'...' if len(answer) > 300 else ''}")


# ============ 4. 多轮对话 Agent ============

async def demo_conversational_agent():
    """
    演示使用 MCP 工具的多轮对话 Agent

    展示：
    - 保持对话上下文
    - 跨轮次引用之前的工具结果
    - 复杂任务的多步骤工具协作
    """
    print("\n" + "=" * 60)
    print("演示 3: 多轮对话 Agent（上下文保持）")
    print("=" * 60)

    async with stdio_client(SERVER_PARAMS) as (read, write):
        async with ClientSession(read, write) as session:
            await session.initialize()

            tools = await load_mcp_tools(session)
            llm_with_tools = llm.bind_tools(tools)
            tool_map = {tool.name: tool for tool in tools}

            # 持久化对话历史
            messages: List[BaseMessage] = [
                SystemMessage(content=(
                    "你是一个智能助手，擅长使用工具帮助用户。"
                    "你可以查询天气、进行数学计算、查看时间、搜索知识和进行单位换算。"
                    "请用中文回答，语气友好自然。"
                ))
            ]

            # 模拟多轮对话
            conversations = [
                "帮我查一下北京今天的天气",
                "上海呢？跟北京比哪个更适合户外活动？",
                "如果我从北京到上海，距离 1318 公里，高铁时速 350 km/h，大约要多久？帮我算一下并转成小时和分钟",
            ]

            for user_input in conversations:
                print(f"\n{'─' * 60}")
                print(f"  用户: {user_input}")

                messages.append(HumanMessage(content=user_input))
                answer = await run_agent_loop(llm_with_tools, tool_map, messages)
                print(f"\n  助手: {answer[:400]}{'...' if len(answer) > 400 else ''}")


# ============ 5. 概念总结 ============

def demo_summary():
    """MCP 核心概念总结"""
    print("\n" + "=" * 60)
    print("MCP (Model Context Protocol) 总结")
    print("=" * 60)
    print("""
【核心架构】

1. MCP Server（服务端）
   - 使用 FastMCP 或底层 Server API 创建
   - 通过 @mcp.tool() 装饰器暴露工具
   - 通过 @mcp.resource() 暴露资源
   - 支持 stdio / SSE / Streamable HTTP 传输

2. MCP Client（客户端）
   - 连接到 MCP Server，建立会话
   - list_tools()  → 发现可用工具
   - call_tool()   → 调用工具
   - list_resources() → 发现可用资源

3. 传输协议
   - stdio: 通过标准输入输出通信，适合本地进程
   - SSE:   基于 HTTP 的服务端事件推送
   - Streamable HTTP: 基于 HTTP 的双向流

【LangChain 集成】

使用 langchain-mcp-adapters 包：
1. load_mcp_tools(session) → 将 MCP 工具转为 LangChain BaseTool
2. llm.bind_tools(tools)  → 绑定工具到 ChatModel
3. 标准的 tool_calls 循环处理工具调用

【最佳实践】

1. 工具设计
   - 每个工具职责单一、描述清晰
   - 参数使用类型注解 + docstring 描述
   - 返回结构化数据（JSON），便于 LLM 解析

2. 服务器管理
   - 一个服务器聚焦一个领域（天气、计算、数据库等）
   - 支持多服务器连接（MultiServerMCPClient）
   - 做好错误处理和超时控制

3. 安全考虑
   - 工具执行要做权限控制
   - 避免暴露危险操作（如 rm、DROP TABLE）
   - 对用户输入做参数校验

【与其他模式结合】

- + Tool Use: MCP 是工具使用的标准化实现
- + ReAct: MCP 工具作为 ReAct 循环中的行动
- + Multi-Agent: 多个 Agent 共享 MCP 工具服务
- + Planning: 规划器根据 MCP 工具列表制定计划
- + Memory: 将 MCP 调用结果存入记忆供后续使用
""")


# ============ 6. 主函数 ============

async def async_main():
    """异步主函数"""
    print("=" * 60)
    print("Chapter 10: MCP (Model Context Protocol) 集成示例")
    print("=" * 60)

    await demo_mcp_basics()
    await demo_langchain_integration()
    await demo_conversational_agent()
    demo_summary()


def main():
    """主函数"""
    asyncio.run(async_main())


if __name__ == "__main__":
    main()
