"""
MCP Server - SSE 传输方式

通过 MCP (Model Context Protocol) 的 SSE (Server-Sent Events) 传输协议暴露工具。

启动方式（独立进程）:
    python ch10/mcp_server_sse.py

服务器将在 http://localhost:8000 启动，提供以下端点：
- GET  /sse      - SSE 连接端点（客户端连接）
- POST /messages - 消息接收端点（客户端发送消息）

暴露的工具：
1. get_stock_price    - 查询股票价格
2. convert_currency   - 货币汇率换算
3. analyze_data       - 简单数据分析
4. generate_report    - 生成文本报告
5. get_market_overview - 市场概览
"""

import json
import random
from datetime import datetime
from contextlib import asynccontextmanager
from collections.abc import AsyncIterator

from mcp.server.fastmcp import FastMCP, Context

# 创建 MCP 服务器实例（SSE 模式，端口 8000）
mcp = FastMCP("agentic-sse-server", port=8000)


# ============ 生命周期管理 ============

@asynccontextmanager
async def app_lifespan(server: FastMCP) -> AsyncIterator[dict]:
    """应用生命周期管理"""
    print(f"[{datetime.now()}] MCP SSE Server 启动中...")
    print(f"  - 服务器名称: {server.name}")
    print(f"  - 监听地址: http://localhost:8000")
    print(f"  - SSE 端点: /sse")
    print(f"  - 消息端点: /messages")

    # 初始化数据
    data = {
        "start_time": datetime.now(),
        "request_count": 0,
    }

    yield data

    # 清理
    print(f"\n[{datetime.now()}] MCP SSE Server 关闭")


mcp = FastMCP("agentic-sse-server", lifespan=app_lifespan)


# ============ 工具定义 ============

@mcp.tool()
def get_stock_price(symbol: str, ctx: Context) -> str:
    """获取指定股票的价格信息

    Args:
        symbol: 股票代码，如 "AAPL"、"TSLA"、"BABA"、"00700"
    """
    # 模拟股票数据
    stock_db = {
        "AAPL": {"name": "Apple Inc.", "price": 175.50, "currency": "USD", "change": 1.25},
        "TSLA": {"name": "Tesla Inc.", "price": 240.30, "currency": "USD", "change": -3.50},
        "BABA": {"name": "Alibaba Group", "price": 85.20, "currency": "USD", "change": 0.80},
        "00700": {"name": "Tencent Holdings", "price": 385.60, "currency": "HKD", "change": 5.20},
        "600519": {"name": "Kweichow Moutai", "price": 1680.00, "currency": "CNY", "change": -12.50},
        "MSFT": {"name": "Microsoft Corp.", "price": 420.15, "currency": "USD", "change": 2.30},
        "GOOGL": {"name": "Alphabet Inc.", "price": 165.80, "currency": "USD", "change": -0.50},
    }

    data = stock_db.get(symbol.upper())
    if data:
        return json.dumps({
            "symbol": symbol.upper(),
            "name": data["name"],
            "price": data["price"],
            "currency": data["currency"],
            "change": data["change"],
            "change_percent": round(data["change"] / (data["price"] - data["change"]) * 100, 2),
            "timestamp": datetime.now().isoformat(),
        }, ensure_ascii=False)

    supported = ", ".join(stock_db.keys())
    return json.dumps({
        "error": f"未找到股票 {symbol}",
        "supported_symbols": supported
    }, ensure_ascii=False)


@mcp.tool()
def convert_currency(amount: float, from_currency: str, to_currency: str) -> str:
    """货币汇率换算

    Args:
        amount: 金额
        from_currency: 源货币代码，如 "USD"、"CNY"、"EUR"
        to_currency: 目标货币代码
    """
    # 模拟汇率（以 USD 为基准）
    rates = {
        "USD": 1.0,
        "CNY": 7.25,
        "EUR": 0.92,
        "JPY": 150.50,
        "GBP": 0.79,
        "HKD": 7.82,
    }

    from_curr = from_currency.upper()
    to_curr = to_currency.upper()

    if from_curr not in rates or to_curr not in rates:
        return json.dumps({
            "error": "不支持的货币",
            "supported": list(rates.keys())
        }, ensure_ascii=False)

    # 换算: amount -> USD -> target
    usd_amount = amount / rates[from_curr]
    result = usd_amount * rates[to_curr]

    return json.dumps({
        "original": f"{amount} {from_curr}",
        "converted": f"{round(result, 2)} {to_curr}",
        "rate": round(rates[to_curr] / rates[from_curr], 6),
        "timestamp": datetime.now().isoformat(),
    }, ensure_ascii=False)


@mcp.tool()
def analyze_data(data: list, operation: str) -> str:
    """对数据进行简单分析

    Args:
        data: 数字列表，如 [10, 20, 30, 40, 50]
        operation: 分析操作，可选 "sum"(求和)、"avg"(平均)、"max"(最大)、"min"(最小)、"std"(标准差)
    """
    if not data:
        return json.dumps({"error": "数据不能为空"}, ensure_ascii=False)

    try:
        numbers = [float(x) for x in data]
    except (ValueError, TypeError):
        return json.dumps({"error": "数据必须是数字"}, ensure_ascii=False)

    operations = {
        "sum": sum(numbers),
        "avg": sum(numbers) / len(numbers),
        "max": max(numbers),
        "min": min(numbers),
        "std": (sum((x - sum(numbers)/len(numbers))**2 for x in numbers) / len(numbers))**0.5,
        "count": len(numbers),
    }

    if operation not in operations:
        return json.dumps({
            "error": f"不支持的操作: {operation}",
            "supported": list(operations.keys())
        }, ensure_ascii=False)

    return json.dumps({
        "data": data,
        "operation": operation,
        "result": round(operations[operation], 4),
        "count": len(numbers),
    }, ensure_ascii=False)


@mcp.tool()
def generate_report(title: str, sections: list) -> str:
    """生成结构化文本报告

    Args:
        title: 报告标题
        sections: 报告章节列表，每个元素是 {"heading": "标题", "content": "内容"}
    """
    report_lines = [
        f"# {title}",
        f"\n生成时间: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}",
        "=" * 50,
    ]

    for i, section in enumerate(sections, 1):
        heading = section.get("heading", f"章节 {i}")
        content = section.get("content", "")
        report_lines.extend([
            f"\n## {i}. {heading}",
            content,
        ])

    report_lines.extend([
        "\n" + "=" * 50,
        "报告生成完成",
    ])

    return json.dumps({
        "title": title,
        "section_count": len(sections),
        "report": "\n".join(report_lines),
    }, ensure_ascii=False)


@mcp.tool()
def get_market_overview() -> str:
    """获取市场概览信息"""
    markets = {
        "US": {"index": "S&P 500", "value": 4850.20, "change": 0.45},
        "CN": {"index": "上证指数", "value": 2950.80, "change": -0.32},
        "HK": {"index": "恒生指数", "value": 16200.50, "change": 1.20},
        "JP": {"index": "日经225", "value": 36500.00, "change": 0.85},
    }

    return json.dumps({
        "timestamp": datetime.now().isoformat(),
        "markets": markets,
        "summary": "全球市场今日表现分化，港股领涨",
    }, ensure_ascii=False)


# ============ 资源定义 ============

@mcp.resource("server://info")
def get_server_info() -> str:
    """获取 MCP 服务器信息"""
    return json.dumps({
        "server_name": "agentic-sse-server",
        "version": "1.0.0",
        "transport": "sse",
        "endpoint": "http://localhost:8000/sse",
        "tools": [
            "get_stock_price", "convert_currency", "analyze_data",
            "generate_report", "get_market_overview"
        ],
        "description": "SSE 传输方式的 MCP 服务器示例",
    }, ensure_ascii=False)


@mcp.resource("data://exchange-rates")
def get_exchange_rates() -> str:
    """获取汇率数据"""
    return json.dumps({
        "base": "USD",
        "rates": {
            "CNY": 7.25,
            "EUR": 0.92,
            "JPY": 150.50,
            "GBP": 0.79,
            "HKD": 7.82,
        },
        "updated_at": datetime.now().isoformat(),
    }, ensure_ascii=False)


# ============ 启动入口 ============

if __name__ == "__main__":
    # FastMCP 内置 SSE 支持（端口已在构造函数中设置）
    mcp.run(transport="sse")
