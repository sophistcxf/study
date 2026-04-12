"""
Chapter 5: Tool Use Pattern (工具使用模式)

工具使用模式是 Agentic Design Patterns 中的核心模式。
它让 LLM 能够调用外部工具（函数、API、命令等）来扩展能力。

核心思想：
- LLM 本身有知识限制（知识截止日期、无法访问实时数据）
- 通过工具，LLM 可以：
  - 执行代码
  - 查询数据库
  - 调用 API
  - 执行终端命令
  - 操作文件系统
- LLM 决定何时使用工具、使用什么工具、传递什么参数

示例场景：智能运维助手，通过执行终端命令诊断和解决问题
"""

import os
import subprocess
import json
import re
from typing import Dict, List, Optional, Callable
from dataclasses import dataclass
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


# ============ 1. 工具定义 ============

@dataclass
class Tool:
    """工具定义"""
    name: str
    description: str
    parameters: Dict
    func: Callable


def execute_command(command: str, timeout: int = 30) -> str:
    """
    执行终端命令并返回结果
    """
    try:
        result = subprocess.run(
            command,
            shell=True,
            capture_output=True,
            text=True,
            timeout=timeout
        )
        output = result.stdout if result.returncode == 0 else result.stderr
        return f"命令: {command}\n返回码: {result.returncode}\n输出:\n{output}"
    except subprocess.TimeoutExpired:
        return f"命令执行超时（{timeout}秒）"
    except Exception as e:
        return f"执行出错: {str(e)}"


def read_file(path: str) -> str:
    """读取文件内容"""
    try:
        with open(path, 'r', encoding='utf-8') as f:
            content = f.read()
        return f"文件 {path} 内容:\n{content[:2000]}"  # 限制长度
    except Exception as e:
        return f"读取文件出错: {str(e)}"


def write_file(path: str, content: str) -> str:
    """写入文件内容"""
    try:
        with open(path, 'w', encoding='utf-8') as f:
            f.write(content)
        return f"成功写入文件: {path}"
    except Exception as e:
        return f"写入文件出错: {str(e)}"


def list_directory(path: str = ".") -> str:
    """列出目录内容"""
    try:
        result = subprocess.run(
            f"ls -la {path}",
            shell=True,
            capture_output=True,
            text=True
        )
        return f"目录 {path} 内容:\n{result.stdout}"
    except Exception as e:
        return f"列出目录出错: {str(e)}"


# 工具注册表
TOOLS = {
    "execute_command": Tool(
        name="execute_command",
        description="执行终端命令（如 ls, cat, grep, ps 等）",
        parameters={"command": "要执行的命令字符串", "timeout": "超时时间（秒，可选，默认30）"},
        func=execute_command
    ),
    "read_file": Tool(
        name="read_file",
        description="读取文件内容",
        parameters={"path": "文件路径"},
        func=read_file
    ),
    "write_file": Tool(
        name="write_file",
        description="写入文件内容",
        parameters={"path": "文件路径", "content": "文件内容"},
        func=write_file
    ),
    "list_directory": Tool(
        name="list_directory",
        description="列出目录内容",
        parameters={"path": "目录路径（可选，默认为当前目录）"},
        func=list_directory
    )
}


# ============ 2. 工具调用解析 ============

def parse_tool_calls(response: str) -> List[Dict]:
    """
    从 LLM 响应中解析工具调用
    格式: ```tool:{tool_name}\n{json_params}\n```
    """
    tool_calls = []
    pattern = r'```tool:(\w+)\n(.*?)\n```'
    matches = re.findall(pattern, response, re.DOTALL)

    for tool_name, params_str in matches:
        if tool_name in TOOLS:
            try:
                params = json.loads(params_str)
                tool_calls.append({
                    "tool": tool_name,
                    "params": params
                })
            except json.JSONDecodeError:
                tool_calls.append({
                    "tool": tool_name,
                    "params": {"raw": params_str},
                    "error": "参数解析失败"
                })

    return tool_calls


def execute_tool_calls(tool_calls: List[Dict]) -> List[Dict]:
    """执行工具调用并返回结果"""
    results = []

    for call in tool_calls:
        tool_name = call["tool"]
        params = call["params"]

        if tool_name in TOOLS:
            tool = TOOLS[tool_name]
            try:
                result = tool.func(**params)
                results.append({
                    "tool": tool_name,
                    "params": params,
                    "result": result,
                    "success": True
                })
            except Exception as e:
                results.append({
                    "tool": tool_name,
                    "params": params,
                    "result": str(e),
                    "success": False
                })
        else:
            results.append({
                "tool": tool_name,
                "params": params,
                "result": f"未知工具: {tool_name}",
                "success": False
            })

    return results


# ============ 3. ReAct 风格 Agent ============

class ToolUseAgent:
    """
    工具使用 Agent
    采用 ReAct 风格：思考 -> 行动（工具调用）-> 观察 -> 重复
    """

    def __init__(self, max_iterations: int = 5):
        self.max_iterations = max_iterations
        self.tools_description = self._build_tools_description()

    def _build_tools_description(self) -> str:
        """构建工具描述"""
        descriptions = []
        for name, tool in TOOLS.items():
            desc = f"""
工具名称: {name}
描述: {tool.description}
参数: {json.dumps(tool.parameters, ensure_ascii=False)}
使用格式:
```tool:{name}
{json.dumps({k: f"<{k}>" for k in tool.parameters.keys()}, ensure_ascii=False, indent=2)}
```
"""
            descriptions.append(desc)
        return "\n".join(descriptions)

    def run(self, task: str, verbose: bool = True) -> str:
        """运行 Agent 完成任务"""
        if verbose:
            print("=" * 70)
            print("Tool Use Pattern 示例 - 智能运维助手")
            print("=" * 70)
            print(f"\n任务: {task}\n")

        # 初始化对话历史
        conversation = []

        for iteration in range(self.max_iterations):
            if verbose:
                print(f"\n{'─' * 70}")
                print(f"第 {iteration + 1} 轮")
                print('─' * 70)

            # 构建提示
            prompt = self._build_prompt(task, conversation)

            # 获取 LLM 响应
            response = llm.invoke(prompt).content

            if verbose:
                print(f"\n🤖 LLM 思考:\n{response[:800]}...\n" if len(response) > 800 else f"\n🤖 LLM 思考:\n{response}\n")

            # 解析工具调用
            tool_calls = parse_tool_calls(response)

            if not tool_calls:
                # 没有工具调用，任务完成
                if verbose:
                    print("✅ 任务完成，无需进一步操作")
                return response

            # 执行工具调用
            if verbose:
                print(f"🔧 执行 {len(tool_calls)} 个工具调用:")

            tool_results = execute_tool_calls(tool_calls)

            for i, result in enumerate(tool_results, 1):
                if verbose:
                    status = "✓" if result["success"] else "✗"
                    print(f"  {status} 工具 {i}: {result['tool']}")
                    print(f"    结果预览: {result['result'][:200]}...\n")

            # 更新对话历史
            conversation.append({
                "iteration": iteration + 1,
                "thought": response,
                "tool_calls": tool_calls,
                "tool_results": tool_results
            })

        if verbose:
            print(f"\n⚠️ 达到最大迭代次数 ({self.max_iterations})")

        return "任务未完成，达到最大迭代次数"

    def _build_prompt(self, task: str, conversation: List[Dict]) -> str:
        """构建提示"""
        history = ""
        for turn in conversation:
            history += f"\n第 {turn['iteration']} 轮:\n"
            history += f"思考: {turn['thought'][:500]}...\n"
            for result in turn['tool_results']:
                history += f"工具 {result['tool']} 结果:\n{result['result'][:500]}...\n"

        prompt = f"""你是一个智能运维助手，可以使用工具来完成任务。

可用工具:
{self.tools_description}

当前任务: {task}

{history}

请分析当前情况，决定下一步行动：
1. 如果需要使用工具，请使用上述格式
2. 如果任务已完成，请直接给出最终答案
3. 请解释你的思考过程

注意：
- 每次可以调用多个工具
- 工具调用格式必须严格遵循示例
- 如果命令可能危险（如 rm -rf），请先询问用户确认

你的回应:"""

        return prompt


# ============ 4. 简单示例：直接工具调用 ============

def simple_tool_example():
    """简单的工具使用示例"""
    print("=" * 70)
    print("简单示例：直接工具调用")
    print("=" * 70)

    # 示例 1：执行命令
    print("\n示例 1: 执行命令 - 查看当前目录")
    result = execute_command("pwd && ls -la")
    print(result)

    # 示例 2：读取文件
    print("\n示例 2: 读取文件 - pyproject.toml")
    result = read_file("pyproject.toml")
    print(result)

    # 示例 3：列出目录
    print("\n示例 3: 列出目录 - ch1/")
    result = list_directory("ch1")
    print(result)


# ============ 5. 演示 ============

def main():
    # 简单示例
    simple_tool_example()

    print("\n" + "=" * 70)
    print("ReAct 风格 Agent 示例")
    print("=" * 70)

    # 创建 Agent
    agent = ToolUseAgent(max_iterations=3)

    # 任务 1：系统诊断
    task1 = "检查当前系统的磁盘使用情况，找出占用空间最大的目录"
    print(f"\n{'='*70}")
    print(f"任务: {task1}")
    print('='*70)
    result1 = agent.run(task1, verbose=True)
    print(f"\n最终结果:\n{result1}")

    # 任务 2：文件分析
    task2 = "读取 ch1/exp1.py 文件，统计代码行数并分析文件结构"
    print(f"\n{'='*70}")
    print(f"任务: {task2}")
    print('='*70)
    result2 = agent.run(task2, verbose=True)
    print(f"\n最终结果:\n{result2}")

    # 总结
    print("\n" + "=" * 70)
    print("Tool Use Pattern 优势总结")
    print("=" * 70)
    print("""
【核心优势】

1. **扩展 LLM 能力**
   - 突破知识截止日期限制
   - 访问实时数据和外部系统
   - 执行实际的操作（文件、命令、API）

2. **解决实际问题**
   - 不仅提供建议，还能实际执行
   - 自动化运维、数据处理、系统管理等任务

3. **灵活性和可扩展性**
   - 易于添加新工具
   - 工具可以是任何 Python 函数

4. **ReAct 模式优势**
   - 思考-行动-观察循环
   - 能处理多步骤复杂任务
   - 每一步都有可解释性

【安全注意事项】

⚠️ 执行终端命令存在安全风险：
- 避免执行 rm -rf 等危险命令
- 限制命令执行范围
- 敏感操作需要用户确认
- 建议设置命令白名单

【适用场景】

- 智能运维助手
- 自动化数据处理
- 代码分析和重构
- 系统诊断和故障排查
- 文件管理和操作
- 与外部 API 集成

【工具类型示例】

| 类型 | 示例 |
|------|------|
| 系统命令 | execute_command, list_directory |
| 文件操作 | read_file, write_file |
| 数据处理 | query_database, call_api |
| 搜索 | web_search, code_search |
| 计算 | python_execute, calculator |
""")


if __name__ == "__main__":
    main()
