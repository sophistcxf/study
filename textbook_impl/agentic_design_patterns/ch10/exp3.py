"""
Chapter 10: JSON-RPC 2.0 客户端示例

本示例演示如何使用纯 Python 实现 JSON-RPC 2.0 客户端，
不依赖任何外部 RPC 框架。

JSON-RPC 2.0 协议要点：
1. 请求格式: {"jsonrpc": "2.0", "method": "方法名", "params": [...], "id": 1}
2. 响应格式: {"jsonrpc": "2.0", "result": ..., "id": 1}
3. 错误格式: {"jsonrpc": "2.0", "error": {"code": -32600, "message": "..."}, "id": 1}
4. 通知请求: 没有 "id" 字段，服务器不返回响应

协议规范: https://www.jsonrpc.org/specification

启动步骤：
1. 先启动服务器: python ch10/jsonrpc_server.py
2. 再运行客户端: python ch10/exp3.py
"""

import json
import urllib.request
import urllib.error
from typing import Any, Optional, List, Dict, Union
from dataclasses import dataclass


# ============ JSON-RPC 客户端实现 ============

@dataclass
class JsonRpcResponse:
    """JSON-RPC 响应包装类"""
    result: Any = None
    error: Optional[Dict] = None
    id: Any = None
    raw_response: Dict = None

    @property
    def is_error(self) -> bool:
        """是否错误响应"""
        return self.error is not None

    @property
    def is_success(self) -> bool:
        """是否成功响应"""
        return self.error is None


class JsonRpcClient:
    """
    JSON-RPC 2.0 客户端

    使用示例:
        client = JsonRpcClient("http://localhost:8001/rpc")
        response = client.call("add", [1, 2])
        print(response.result)  # 3
    """

    def __init__(self, endpoint: str, timeout: int = 30):
        """
        初始化客户端

        Args:
            endpoint: JSON-RPC 服务器地址，如 "http://localhost:8001/rpc"
            timeout: 请求超时时间（秒）
        """
        self.endpoint = endpoint
        self.timeout = timeout
        self._request_id = 0

    def _get_next_id(self) -> int:
        """获取下一个请求 ID"""
        self._request_id += 1
        return self._request_id

    def call(self, method: str, params: Union[List, Dict] = None, 
             request_id: Any = None) -> JsonRpcResponse:
        """
        同步调用远程方法

        Args:
            method: 方法名
            params: 参数列表或参数字典
            request_id: 自定义请求 ID（默认自动生成）

        Returns:
            JsonRpcResponse 对象
        """
        if request_id is None:
            request_id = self._get_next_id()

        # 构建请求体
        request = {
            "jsonrpc": "2.0",
            "method": method,
            "id": request_id,
        }
        if params is not None:
            request["params"] = params

        # 发送 HTTP 请求
        try:
            request_body = json.dumps(request, ensure_ascii=False).encode("utf-8")
            
            req = urllib.request.Request(
                self.endpoint,
                data=request_body,
                headers={
                    "Content-Type": "application/json",
                    "Accept": "application/json",
                },
                method="POST",
            )

            with urllib.request.urlopen(req, timeout=self.timeout) as response:
                # 检查状态码
                if response.status == 204:
                    # 通知请求，无响应体
                    return JsonRpcResponse(result=None, id=request_id)

                # 解析响应
                response_body = response.read().decode("utf-8")
                response_data = json.loads(response_body)

                return self._parse_response(response_data)

        except urllib.error.HTTPError as e:
            return JsonRpcResponse(
                error={"code": e.code, "message": f"HTTP 错误: {e.reason}"},
                id=request_id,
            )
        except urllib.error.URLError as e:
            return JsonRpcResponse(
                error={"code": -32000, "message": f"连接错误: {str(e.reason)}"},
                id=request_id,
            )
        except json.JSONDecodeError as e:
            return JsonRpcResponse(
                error={"code": -32700, "message": f"JSON 解析错误: {str(e)}"},
                id=request_id,
            )
        except Exception as e:
            return JsonRpcResponse(
                error={"code": -32603, "message": f"客户端错误: {str(e)}"},
                id=request_id,
            )

    def notify(self, method: str, params: Union[List, Dict] = None) -> None:
        """
        发送通知请求（无响应）

        Args:
            method: 方法名
            params: 参数
        """
        request = {
            "jsonrpc": "2.0",
            "method": method,
        }
        if params is not None:
            request["params"] = params

        try:
            request_body = json.dumps(request, ensure_ascii=False).encode("utf-8")
            req = urllib.request.Request(
                self.endpoint,
                data=request_body,
                headers={"Content-Type": "application/json"},
                method="POST",
            )
            urllib.request.urlopen(req, timeout=self.timeout)
        except Exception:
            pass  # 通知请求不关心响应

    def batch_call(self, calls: List[tuple]) -> List[JsonRpcResponse]:
        """
        批量调用多个方法

        Args:
            calls: 调用列表，每个元素是 (method, params) 元组

        Returns:
            响应列表
        """
        # 构建批量请求
        batch_request = []
        for i, (method, params) in enumerate(calls):
            request = {
                "jsonrpc": "2.0",
                "method": method,
                "id": self._get_next_id(),
            }
            if params is not None:
                request["params"] = params
            batch_request.append(request)

        try:
            request_body = json.dumps(batch_request, ensure_ascii=False).encode("utf-8")
            req = urllib.request.Request(
                self.endpoint,
                data=request_body,
                headers={"Content-Type": "application/json"},
                method="POST",
            )

            with urllib.request.urlopen(req, timeout=self.timeout) as response:
                if response.status == 204:
                    return []

                response_body = response.read().decode("utf-8")
                response_data = json.loads(response_body)

                if not isinstance(response_data, list):
                    return [self._parse_response(response_data)]

                return [self._parse_response(resp) for resp in response_data]

        except Exception as e:
            return [
                JsonRpcResponse(
                    error={"code": -32603, "message": f"批量调用错误: {str(e)}"}
                )
            ]

    def _parse_response(self, data: Dict) -> JsonRpcResponse:
        """解析响应数据"""
        return JsonRpcResponse(
            result=data.get("result"),
            error=data.get("error"),
            id=data.get("id"),
            raw_response=data,
        )


# ============ 演示 ============

def demo_basic_calls():
    """演示基本调用"""
    print("=" * 60)
    print("演示 1: 基本 JSON-RPC 调用")
    print("=" * 60)

    client = JsonRpcClient("http://localhost:8001/rpc")

    # 1. 加法
    print("\n[调用] add(10, 20)")
    resp = client.call("add", [10, 20])
    if resp.is_success:
        print(f"  → 结果: {resp.result}")
    else:
        print(f"  → 错误: {resp.error}")

    # 2. 减法
    print("\n[调用] subtract(100, 30)")
    resp = client.call("subtract", [100, 30])
    print(f"  → 结果: {resp.result}")

    # 3. 乘法
    print("\n[调用] multiply(5, 6)")
    resp = client.call("multiply", [5, 6])
    print(f"  → 结果: {resp.result}")

    # 4. 除法
    print("\n[调用] divide(100, 4)")
    resp = client.call("divide", [100, 4])
    print(f"  → 结果: {resp.result}")

    # 5. 除零错误
    print("\n[调用] divide(100, 0) - 测试错误处理")
    resp = client.call("divide", [100, 0])
    print(f"  → 错误码: {resp.error['code']}")
    print(f"  → 错误信息: {resp.error['message']}")


def demo_named_params():
    """演示命名参数"""
    print("\n" + "=" * 60)
    print("演示 2: 使用命名参数")
    print("=" * 60)

    client = JsonRpcClient("http://localhost:8001/rpc")

    # 使用位置参数
    print("\n[调用] greet('张三') - 位置参数")
    resp = client.call("greet", ["张三"])
    print(f"  → {resp.result}")

    # 使用命名参数
    print("\n[调用] greet(name='李四', greeting='早上好') - 命名参数")
    resp = client.call("greet", {"name": "李四", "greeting": "早上好"})
    print(f"  → {resp.result}")


def demo_batch_calls():
    """演示批量调用"""
    print("\n" + "=" * 60)
    print("演示 3: 批量调用")
    print("=" * 60)

    client = JsonRpcClient("http://localhost:8001/rpc")

    print("\n[批量调用] 同时执行多个计算...")
    calls = [
        ("add", [1, 2]),
        ("multiply", [3, 4]),
        ("divide", [100, 5]),
        ("greet", ["批量调用"]),
    ]

    responses = client.batch_call(calls)

    for i, ((method, params), resp) in enumerate(zip(calls, responses), 1):
        if resp.is_success:
            print(f"  {i}. {method}{params} = {resp.result}")
        else:
            print(f"  {i}. {method}{params} - 错误: {resp.error['message']}")


def demo_error_handling():
    """演示错误处理"""
    print("\n" + "=" * 60)
    print("演示 4: 错误处理")
    print("=" * 60)

    client = JsonRpcClient("http://localhost:8001/rpc")

    # 1. 方法不存在
    print("\n[调用] 不存在的方法 'foo'")
    resp = client.call("foo", [1, 2, 3])
    print(f"  → 错误码: {resp.error['code']}")
    print(f"  → 错误信息: {resp.error['message']}")

    # 2. 参数错误
    print("\n[调用] add() - 缺少参数")
    resp = client.call("add", [])
    print(f"  → 错误码: {resp.error['code']}")
    print(f"  → 错误信息: {resp.error['message']}")

    # 3. 无效表达式
    print("\n[调用] calculate('invalid') - 无效表达式")
    resp = client.call("calculate", ["invalid_var + 1"])
    print(f"  → 错误码: {resp.error['code']}")
    print(f"  → 错误信息: {resp.error['message']}")


def demo_server_info():
    """演示获取服务器信息"""
    print("\n" + "=" * 60)
    print("演示 5: 获取服务器信息")
    print("=" * 60)

    client = JsonRpcClient("http://localhost:8001/rpc")

    # 使用 GET 请求获取信息
    print("\n[GET] http://localhost:8001/")
    try:
        req = urllib.request.Request("http://localhost:8001/", method="GET")
        with urllib.request.urlopen(req, timeout=10) as response:
            data = json.loads(response.read().decode("utf-8"))
            print(f"  → 服务器: {data['server']}")
            print(f"  → 版本: {data['version']}")
            print(f"  → 可用方法: {', '.join(data['methods'])}")
    except Exception as e:
        print(f"  → 错误: {e}")

    # RPC 调用获取信息
    print("\n[调用] get_info()")
    resp = client.call("get_info")
    if resp.is_success:
        info = resp.result
        print(f"  → 服务器: {info['server']}")
        print(f"  → 版本: {info['version']}")
        print(f"  → 当前时间: {info['time']}")
        print(f"  → 可用方法数: {len(info['methods'])}")

    # 列出所有方法
    print("\n[调用] system.listMethods()")
    resp = client.call("system.listMethods")
    if resp.is_success:
        print(f"  → 可用方法: {', '.join(resp.result)}")


def demo_raw_json():
    """演示原始 JSON-RPC 消息"""
    print("\n" + "=" * 60)
    print("演示 6: 原始 JSON-RPC 消息")
    print("=" * 60)

    print("\n请求示例:")
    request = {
        "jsonrpc": "2.0",
        "method": "add",
        "params": [10, 20],
        "id": 1
    }
    print(json.dumps(request, indent=2, ensure_ascii=False))

    print("\n成功响应示例:")
    success_response = {
        "jsonrpc": "2.0",
        "result": 30,
        "id": 1
    }
    print(json.dumps(success_response, indent=2, ensure_ascii=False))

    print("\n错误响应示例:")
    error_response = {
        "jsonrpc": "2.0",
        "error": {
            "code": -32601,
            "message": "方法 'foo' 未找到"
        },
        "id": 1
    }
    print(json.dumps(error_response, indent=2, ensure_ascii=False))

    print("\n通知请求示例（无 id）:")
    notification = {
        "jsonrpc": "2.0",
        "method": "log",
        "params": ["用户登录"]
    }
    print(json.dumps(notification, indent=2, ensure_ascii=False))


def demo_comparison():
    """JSON-RPC 与其他协议对比"""
    print("\n" + "=" * 60)
    print("JSON-RPC 2.0 与其他协议对比")
    print("=" * 60)
    print("""
【JSON-RPC 2.0】

特点：
- 轻量级远程调用协议
- 基于 JSON 格式
- 支持多种传输（HTTP、WebSocket、TCP 等）
- 支持批量调用
- 有标准错误码

优点：
- 简单易用，无需 IDL
- 跨语言支持好
- 可读性强
- 适合 Web 场景

缺点：
- 无内置服务发现
- 无类型安全（运行时检查）
- 性能不如二进制协议

【gRPC】

特点：
- 基于 HTTP/2 和 Protocol Buffers
- 强类型，需定义 .proto 文件
- 支持流式调用
- 二进制传输，性能高

对比：
| 特性       | JSON-RPC 2.0 | gRPC       |
|-----------|-------------|------------|
| 数据格式   | JSON        | Protobuf   |
| 类型安全   | 弱          | 强         |
| 可读性     | 好          | 差（二进制）|
| 性能       | 一般        | 高         |
| 浏览器支持 | 好          | 需 gRPC-Web|
| 复杂度     | 低          | 高         |

【REST API】

特点：
- 基于 HTTP 方法和 URL
- 资源导向设计
- 无固定请求格式

对比：
| 特性       | JSON-RPC 2.0 | REST       |
|-----------|-------------|------------|
| 设计范式   | 方法调用     | 资源操作    |
| URL 设计   | 统一端点     | 多个端点    |
| 批量操作   | 原生支持     | 需额外设计  |
| 错误处理   | 标准错误码   | HTTP 状态码 |
| 适用场景   | RPC 调用     | CRUD 操作   |

【选择建议】

使用 JSON-RPC 当：
- 需要简单快速的 RPC 方案
- 跨语言互操作
- 调试时需要可读的消息
- 不需要极致性能

使用 gRPC 当：
- 需要高性能
- 需要强类型约束
- 需要流式调用
- 团队熟悉 Protobuf

使用 REST 当：
- 资源导向的 API 设计
- 需要利用 HTTP 缓存
- 对外公开 API
""")


# ============ 主函数 ============

def main():
    """主函数"""
    print("=" * 60)
    print("Chapter 10: JSON-RPC 2.0 客户端示例")
    print("=" * 60)
    print("\n请确保服务器已启动:")
    print("  python ch10/jsonrpc_server.py")
    print()

    # 检查服务器是否可连接
    try:
        req = urllib.request.Request("http://localhost:8001/", method="GET")
        with urllib.request.urlopen(req, timeout=5) as response:
            if response.status == 200:
                print("✓ 服务器连接成功\n")
    except Exception as e:
        print(f"✗ 无法连接服务器: {e}")
        print("\n请先启动服务器:")
        print("  python ch10/jsonrpc_server.py")
        return

    demo_basic_calls()
    demo_named_params()
    demo_batch_calls()
    demo_error_handling()
    demo_server_info()
    demo_raw_json()
    demo_comparison()


if __name__ == "__main__":
    main()
