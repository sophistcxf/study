"""
MCP Server - 演示用工具服务器

通过 MCP (Model Context Protocol) 的 stdio 传输协议暴露以下工具：
1. get_weather    - 查询城市天气
2. calculate      - 数学表达式计算
3. get_current_time - 获取当前时间
4. search_knowledge - 知识库搜索
5. unit_convert    - 单位换算

启动方式（由客户端通过 stdio 自动启动）:
    python ch10/mcp_server.py
"""

import json
import math
from datetime import datetime

from mcp.server.fastmcp import FastMCP

# 创建 MCP 服务器实例
mcp = FastMCP("agentic-demo-server")


# ============ 工具定义 ============

@mcp.tool()
def get_weather(city: str) -> str:
    """获取指定城市的天气信息

    Args:
        city: 城市名称，如 "北京"、"上海"、"广州"
    """
    weather_db = {
        "北京": {"temp": 22, "condition": "晴", "humidity": 45, "wind": "北风3级", "aqi": 75},
        "上海": {"temp": 25, "condition": "多云", "humidity": 65, "wind": "东南风2级", "aqi": 60},
        "广州": {"temp": 30, "condition": "阵雨", "humidity": 80, "wind": "南风2级", "aqi": 55},
        "深圳": {"temp": 29, "condition": "多云转晴", "humidity": 75, "wind": "西南风3级", "aqi": 50},
        "杭州": {"temp": 24, "condition": "晴", "humidity": 55, "wind": "东风2级", "aqi": 65},
        "成都": {"temp": 20, "condition": "阴", "humidity": 70, "wind": "微风", "aqi": 90},
        "武汉": {"temp": 26, "condition": "晴转多云", "humidity": 60, "wind": "南风2级", "aqi": 70},
        "西安": {"temp": 18, "condition": "多云", "humidity": 40, "wind": "西北风3级", "aqi": 85},
    }

    data = weather_db.get(city)
    if data:
        return json.dumps({
            "city": city,
            "temperature": f"{data['temp']}°C",
            "condition": data["condition"],
            "humidity": f"{data['humidity']}%",
            "wind": data["wind"],
            "aqi": data["aqi"],
        }, ensure_ascii=False)

    supported = ", ".join(weather_db.keys())
    return json.dumps({
        "error": f"暂无 {city} 的天气数据",
        "supported_cities": supported
    }, ensure_ascii=False)


@mcp.tool()
def calculate(expression: str) -> str:
    """安全地计算数学表达式

    Args:
        expression: 数学表达式，如 "2 + 3 * 4"、"sqrt(16)"、"sin(3.14)"、"log(100)"
    """
    safe_functions = {
        "abs": abs, "round": round, "min": min, "max": max, "pow": pow,
        "sqrt": math.sqrt, "sin": math.sin, "cos": math.cos, "tan": math.tan,
        "log": math.log, "log2": math.log2, "log10": math.log10,
        "pi": math.pi, "e": math.e,
        "ceil": math.ceil, "floor": math.floor,
        "factorial": math.factorial,
    }

    try:
        result = eval(expression, {"__builtins__": {}}, safe_functions)
        return json.dumps({
            "expression": expression,
            "result": result
        }, ensure_ascii=False)
    except Exception as e:
        return json.dumps({
            "expression": expression,
            "error": str(e)
        }, ensure_ascii=False)


@mcp.tool()
def get_current_time() -> str:
    """获取当前系统时间和日期"""
    now = datetime.now()
    weekdays = ["周一", "周二", "周三", "周四", "周五", "周六", "周日"]
    return json.dumps({
        "datetime": now.strftime("%Y-%m-%d %H:%M:%S"),
        "date": now.strftime("%Y-%m-%d"),
        "time": now.strftime("%H:%M:%S"),
        "weekday": weekdays[now.weekday()],
        "timestamp": int(now.timestamp()),
    }, ensure_ascii=False)


@mcp.tool()
def search_knowledge(query: str, topic: str = "general") -> str:
    """搜索知识库中的信息

    Args:
        query: 搜索关键词
        topic: 主题分类，可选 "general"(通用)、"tech"(技术)、"science"(科学)
    """
    knowledge_base = {
        "tech": {
            "python": "Python 是一种高级通用编程语言，以简洁可读的语法著称。由 Guido van Rossum 于 1991 年创建，广泛用于 Web 开发、数据科学、AI 和自动化。",
            "mcp": "MCP (Model Context Protocol) 是由 Anthropic 提出的开放协议，用于标准化 AI 模型与外部工具和数据源的连接。它采用客户端-服务器架构，支持 stdio、SSE 和 Streamable HTTP 传输。",
            "langchain": "LangChain 是一个用于开发 LLM 驱动应用的开源框架，提供链式调用、Agent、工具集成、记忆管理等核心功能，支持多种模型和数据源。",
            "agent": "AI Agent（智能体）是一个能自主感知环境、做出决策并执行行动的 AI 系统。它通常具备推理、规划、工具使用和记忆等能力。",
            "rag": "RAG (Retrieval-Augmented Generation) 是一种结合检索和生成的 AI 技术，通过从外部知识库检索相关信息来增强 LLM 的回答质量。",
        },
        "science": {
            "physics": "物理学是研究自然界基本规律的科学，包括经典力学、电磁学、热力学、量子力学和相对论等分支。",
            "math": "数学是研究数量、结构、变化和空间的学科，被称为科学的语言，是所有自然科学和工程学的基础。",
            "biology": "生物学是研究生命现象和生命活动规律的科学，涵盖分子生物学、细胞生物学、生态学、遗传学等领域。",
            "chemistry": "化学是研究物质的组成、结构、性质和变化规律的科学，连接物理学和生物学的桥梁学科。",
        },
        "general": {
            "beijing": "北京是中国首都，拥有 3000 多年建城史和 800 多年建都史，是中国政治、文化、国际交流和科技创新中心。",
            "shanghai": "上海是中国最大的经济中心城市和全球金融中心之一，位于长江入海口，是国际航运和贸易中心。",
            "hangzhou": "杭州是浙江省省会，以西湖著称，也是中国互联网经济重镇，阿里巴巴等科技巨头总部所在地。",
        }
    }

    results = []
    search_topics = [topic] if topic in knowledge_base else knowledge_base.keys()

    for t in search_topics:
        if t in knowledge_base:
            for key, value in knowledge_base[t].items():
                if query.lower() in key.lower() or query.lower() in value.lower():
                    results.append({"topic": t, "key": key, "content": value})

    if results:
        return json.dumps({"query": query, "results": results}, ensure_ascii=False)

    return json.dumps({
        "query": query,
        "results": [],
        "message": f"未找到与 '{query}' 相关的信息"
    }, ensure_ascii=False)


@mcp.tool()
def unit_convert(value: float, from_unit: str, to_unit: str) -> str:
    """单位换算工具

    Args:
        value: 数值
        from_unit: 源单位，如 "km", "mile", "kg", "lb", "celsius", "fahrenheit"
        to_unit: 目标单位
    """
    conversions = {
        ("km", "mile"): lambda v: v * 0.621371,
        ("mile", "km"): lambda v: v * 1.60934,
        ("kg", "lb"): lambda v: v * 2.20462,
        ("lb", "kg"): lambda v: v * 0.453592,
        ("m", "ft"): lambda v: v * 3.28084,
        ("ft", "m"): lambda v: v * 0.3048,
        ("celsius", "fahrenheit"): lambda v: v * 9 / 5 + 32,
        ("fahrenheit", "celsius"): lambda v: (v - 32) * 5 / 9,
        ("km/h", "m/s"): lambda v: v / 3.6,
        ("m/s", "km/h"): lambda v: v * 3.6,
        ("hour", "minute"): lambda v: v * 60,
        ("minute", "hour"): lambda v: v / 60,
        ("minute", "second"): lambda v: v * 60,
        ("second", "minute"): lambda v: v / 60,
    }

    key = (from_unit.lower(), to_unit.lower())
    if key in conversions:
        result = conversions[key](value)
        return json.dumps({
            "original": f"{value} {from_unit}",
            "converted": f"{round(result, 4)} {to_unit}",
        }, ensure_ascii=False)

    return json.dumps({
        "error": f"不支持 {from_unit} → {to_unit} 的转换",
        "supported": [f"{f} → {t}" for f, t in conversions.keys()]
    }, ensure_ascii=False)


# ============ 资源定义 ============

@mcp.resource("server://info")
def get_server_info() -> str:
    """获取 MCP 服务器信息"""
    return json.dumps({
        "server_name": "agentic-demo-server",
        "version": "1.0.0",
        "tools": [
            "get_weather", "calculate", "get_current_time",
            "search_knowledge", "unit_convert"
        ],
        "transport": "stdio",
        "description": "Agentic Design Patterns 教学用 MCP 服务器",
    }, ensure_ascii=False)


# ============ 启动入口 ============

if __name__ == "__main__":
    mcp.run(transport="stdio")
