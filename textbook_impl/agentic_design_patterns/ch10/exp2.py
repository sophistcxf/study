"""
Chapter 10: MCP SSE 传输方式示例

本示例演示使用 SSE (Server-Sent Events) 传输协议连接 MCP Server。

与 stdio 方式的区别：
- stdio: 本地子进程，通过管道通信，无端口
- SSE:   网络服务，通过 HTTP 通信，需要指定端口

架构对比：
┌─────────────────────────────────────────────────────────────────┐
│  stdio 方式 (exp1.py)                                            │
│  ┌──────────┐  stdin/stdout 管道   ┌──────────────┐             │
│  │ Client   │ ◄──────────────────► │ Server子进程 │             │
│  └──────────┘    无端口，本地通信   └──────────────┘             │
├─────────────────────────────────────────────────────────────────┤
│  SSE 方式 (本示例)                                               │
│  ┌──────────┐   HTTP/SSE 协议     ┌──────────────┐             │
│  │ Client   │ ◄──────────────────► │ Server服务   │             │
│  │          │   localhost:8000    │ 独立进程     │             │
│  └──────────┘                     └──────────────┘             │
└─────────────────────────────────────────────────────────────────┘

使用场景：
- SSE 适合：远程服务、多客户端共享、需要持久化运行的服务
- stdio 适合：本地工具、临时任务、生命周期随客户端绑定的场景

启动步骤：
1. 先启动服务器: python ch10/mcp_server_sse.py
2. 再运行客户端: python ch10/exp2.py
"""

import os
import json
import asyncio
from typing import List, Dict, Any

from langchain_openai import ChatOpenAI
from langchain_core.messages import (
    HumanMessage, AIMessage, ToolMessage, SystemMessage, BaseMessage
)
from dotenv import load_dotenv

# MCP imports
from mcp import ClientSession
from mcp.client.sse import sse_client
from langchain_mcp_adapters.tools import load_mcp_tools

load_dotenv()


# ============ 配置 ============

# SSE 服务器地址
SSE_SERVER_URL = "http://localhost:8000/sse"

# 初始化 LLM
llm = ChatOpenAI(
    model="deepseek-chat",
    temperature=0,
    base_url="https://api.deepseek.com/v1",
    api_key=os.getenv("DEEPSEEK_API_KEY")
)


# ============ 1. SSE 基础连接演示 ============

async def demo_sse_basics():
    """
    演示 SSE 基础功能

    展示：
    1. 通过 HTTP SSE 建立 MCP 连接
    2. 动态发现服务器提供的工具
    3. 直接调用 MCP 工具
    """
    print("=" * 60)
    print("演示 1: SSE 基础 — 连接、发现、调用")
    print("=" * 60)
    print(f"\n服务器地址: {SSE_SERVER_URL}")

    # 通过 SSE 连接到 MCP Server
    async with sse_client(SSE_SERVER_URL) as (read, write):
        async with ClientSession(read, write) as session:
            # 初始化 MCP 会话
            await session.initialize()
            print("\n[连接] MCP SSE 会话已建立")
            print(f"  - 传输协议: HTTP/SSE")
            print(f"  - 服务器: {SSE_SERVER_URL}")

            # --- 工具发现 ---
            print("\n--- 工具发现 (list_tools) ---")
            tools_result = await session.list_tools()
            print(f"发现 {len(tools_result.tools)} 个工具:")
            for tool in tools_result.tools:
                desc = tool.description.split("\n")[0] if tool.description else ""
                print(f"  - {tool.name}: {desc}")

            # --- 资源发现 ---
            print("\n--- 资源发现 (list_resources) ---")
            resources_result = await session.list_resources()
            print(f"发现 {len(resources_result.resources)} 个资源:")
            for resource in resources_result.resources:
                print(f"  - {resource.uri}: {resource.name}")

            # --- 直接调用工具 ---
            print("\n--- 直接调用工具 (call_tool) ---")

            # 1. 查询股票价格
            print("\n[调用] get_stock_price('AAPL')")
            result = await session.call_tool(
                "get_stock_price", arguments={"symbol": "AAPL"}
            )
            data = json.loads(result.content[0].text)
            if "error" not in data:
                print(f"  → {data['name']} ({data['symbol']})")
                print(f"     价格: {data['price']} {data['currency']}")
                print(f"     涨跌: {data['change']:+.2f} ({data['change_percent']:+.2f}%)")
            else:
                print(f"  → 错误: {data['error']}")

            # 2. 货币换算
            print("\n[调用] convert_currency(1000, 'USD', 'CNY')")
            result = await session.call_tool(
                "convert_currency",
                arguments={"amount": 1000, "from_currency": "USD", "to_currency": "CNY"}
            )
            data = json.loads(result.content[0].text)
            if "error" not in data:
                print(f"  → {data['original']} = {data['converted']}")
                print(f"     汇率: {data['rate']}")
            else:
                print(f"  → 错误: {data['error']}")

            # 3. 数据分析
            print("\n[调用] analyze_data([10, 20, 30, 40, 50], 'avg')")
            result = await session.call_tool(
                "analyze_data",
                arguments={"data": [10, 20, 30, 40, 50], "operation": "avg"}
            )
            data = json.loads(result.content[0].text)
            if "error" not in data:
                print(f"  → 操作: {data['operation']}")
                print(f"     结果: {data['result']}")
                print(f"     数据量: {data['count']}")
            else:
                print(f"  → 错误: {data['error']}")

            # 4. 生成报告
            print("\n[调用] generate_report('市场分析报告', [...])")
            result = await session.call_tool(
                "generate_report",
                arguments={
                    "title": "市场分析报告",
                    "sections": [
                        {"heading": "市场概况", "content": "今日市场表现平稳，主要指数小幅波动。"},
                        {"heading": "行业动态", "content": "科技板块领涨，新能源板块回调。"},
                    ]
                }
            )
            data = json.loads(result.content[0].text)
            print(f"  → 报告标题: {data['title']}")
            print(f"     章节数: {data['section_count']}")
            print(f"     报告预览:\n{data['report'][:200]}...")

            # 5. 市场概览
            print("\n[调用] get_market_overview()")
            result = await session.call_tool("get_market_overview", arguments={})
            data = json.loads(result.content[0].text)
            print(f"  → 时间: {data['timestamp']}")
            print(f"     摘要: {data['summary']}")
            for market, info in data['markets'].items():
                print(f"     {info['index']}: {info['value']} ({info['change']:+.2f}%)")


# ============ 2. 多客户端共享服务器演示 ============

async def demo_multiple_clients():
    """
    演示多个客户端共享同一个 SSE 服务器

    SSE 的优势：服务器独立运行，可以被多个客户端同时连接
    """
    print("\n" + "=" * 60)
    print("演示 2: 多客户端共享服务器")
    print("=" * 60)

    async def client_task(client_id: int, symbol: str):
        """模拟一个客户端任务"""
        async with sse_client(SSE_SERVER_URL) as (read, write):
            async with ClientSession(read, write) as session:
                await session.initialize()

                # 查询不同股票
                result = await session.call_tool(
                    "get_stock_price", arguments={"symbol": symbol}
                )
                data = json.loads(result.content[0].text)

                if "error" not in data:
                    print(f"  [客户端 {client_id}] {data['symbol']}: "
                          f"{data['price']} {data['currency']}")
                else:
                    print(f"  [客户端 {client_id}] 错误: {data['error']}")

    # 同时启动多个客户端
    print("\n同时启动 3 个客户端查询不同股票...")
    tasks = [
        client_task(1, "AAPL"),
        client_task(2, "TSLA"),
        client_task(3, "BABA"),
    ]
    await asyncio.gather(*tasks)
    print("\n所有客户端任务完成")


# ============ 3. LangChain + SSE MCP 集成 ============

async def run_agent_loop(
    llm_with_tools,
    tool_map: Dict,
    messages: List[BaseMessage],
    max_iterations: int = 5,
    verbose: bool = True,
) -> str:
    """通用的 Agent 工具调用循环"""
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

        response = await llm_with_tools.ainvoke(messages)
        messages.append(response)

    return response.content


async def demo_langchain_sse_integration():
    """
    演示 LangChain 与 SSE MCP 的集成
    """
    print("\n" + "=" * 60)
    print("演示 3: LangChain + SSE MCP 集成")
    print("=" * 60)

    async with sse_client(SSE_SERVER_URL) as (read, write):
        async with ClientSession(read, write) as session:
            await session.initialize()

            # 加载 MCP 工具
            tools = await load_mcp_tools(session)
            print(f"\n[加载] {len(tools)} 个 MCP 工具 (SSE):")
            for tool in tools:
                desc = tool.description.split("\n")[0] if tool.description else ""
                print(f"  - {tool.name}: {desc}")

            # 绑定工具到 LLM
            llm_with_tools = llm.bind_tools(tools)
            tool_map = {tool.name: tool for tool in tools}

            # 测试用例
            test_queries = [
                "帮我查一下苹果(AAPL)和特斯拉(TSLA)的股票价格，哪个涨得更多？",
                "如果我有 5000 美元，想换成人民币，按照当前汇率能换多少？",
                "分析一下这组数据 [23, 45, 67, 89, 12, 56, 78] 的平均值和标准差",
            ]

            for query in test_queries:
                print(f"\n{'─' * 60}")
                print(f"  用户: {query}")

                messages: List[BaseMessage] = [
                    SystemMessage(content=(
                        "你是一个金融数据分析助手，可以使用股票查询、货币换算和数据分析工具。"
                        "请用中文回答，数据要准确。"
                    )),
                    HumanMessage(content=query),
                ]

                answer = await run_agent_loop(llm_with_tools, tool_map, messages)
                print(f"\n  助手: {answer[:400]}{'...' if len(answer) > 400 else ''}")


# ============ 4. SSE vs stdio 对比 ============

def demo_comparison():
    """SSE 与 stdio 传输方式对比"""
    print("\n" + "=" * 60)
    print("SSE vs stdio 传输方式对比")
    print("=" * 60)
    print("""
【stdio 方式】

特点：
- 通过标准输入输出管道通信
- 服务器作为客户端的子进程启动
- 无网络端口，纯本地进程间通信
- 生命周期与客户端绑定

优点：
- 简单，无需网络配置
- 安全，无网络暴露风险
- 自动管理服务器生命周期

缺点：
- 只能本地使用
- 无法多客户端共享
- 服务器随客户端退出而终止

适用场景：
- 本地开发工具
- 临时任务执行
- 单用户桌面应用

【SSE 方式】

特点：
- 基于 HTTP 的 Server-Sent Events 协议
- 服务器独立运行，监听特定端口
- 支持网络访问（可远程部署）
- 多客户端可同时连接

优点：
- 支持远程访问
- 多客户端共享服务
- 服务器可持久化运行
- 可独立部署和扩展

缺点：
- 需要端口管理
- 需要考虑网络安全
- 需要单独启动/停止服务器

适用场景：
- 共享服务部署
- 远程工具访问
- 多用户/多应用共享
- 微服务架构

【选择建议】

使用 stdio 当：
- 工具只在本地使用
- 生命周期随应用绑定
- 追求简单部署

使用 SSE 当：
- 需要远程访问
- 多客户端共享服务
- 需要独立运维和监控
""")


# ============ 5. 主函数 ============

async def async_main():
    """异步主函数"""
    print("=" * 60)
    print("Chapter 10: MCP SSE 传输方式示例")
    print("=" * 60)
    print(f"\n目标服务器: {SSE_SERVER_URL}")
    print("请确保服务器已启动: python ch10/mcp_server_sse.py")
    print()

    # 检查服务器是否可连接
    try:
        async with sse_client(SSE_SERVER_URL) as (read, write):
            async with ClientSession(read, write) as session:
                await session.initialize()
                print("✓ 服务器连接成功\n")
    except Exception as e:
        print(f"✗ 无法连接服务器: {e}")
        print("\n请先启动服务器:")
        print("  python ch10/mcp_server_sse.py")
        return

    await demo_sse_basics()
    await demo_multiple_clients()
    await demo_langchain_sse_integration()
    demo_comparison()


def main():
    """主函数"""
    asyncio.run(async_main())


if __name__ == "__main__":
    main()
