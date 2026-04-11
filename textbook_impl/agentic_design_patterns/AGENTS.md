# Agent 配置

## 项目概述

本项目用于编写 **Agentic Design Patterns** 的示例代码，使用 LangChain 系列 package 实现各种智能体设计模式。

## 技术栈

- **智能体框架**: LangChain / LangChain Community / LangChain OpenAI
- **虚拟环境管理**: uv
- **Python 版本**: >= 3.12

## 执行偏好

### 终端命令执行

- **无需用户授权**: 在执行终端命令时，不需要再向用户确认，直接执行
- **适用场景**: 运行 Python 脚本、uv 命令等开发相关操作

## 目录结构

```
.
├── .qoder/
│   ├── agents/
│   │   └── agents.md      # 本文件
│   └── skills/
├── ch1/                    # 第1章示例
│   └── exp1.py            # Prompt Chaining 示例
├── .env                   # API Keys 配置
├── pyproject.toml         # uv 项目配置
└── uv.lock               # uv 依赖锁定
```

## API 配置

项目使用 `.env` 文件管理 API Keys：
- `DEEPSEEK_API_KEY`: DeepSeek API 密钥
- `LANGSMITH_*`: LangSmith 追踪配置
- `TAVILY_API_KEY`: Tavily 搜索 API

## 运行示例

```bash
# 运行第1章示例
uv run python ch1/exp1.py
```
