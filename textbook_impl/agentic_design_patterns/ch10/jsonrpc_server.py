"""
JSON-RPC 2.0 服务器示例

使用 Python 内置的 http.server 和 json 模块实现，不依赖外部 RPC 框架。
展示 JSON-RPC 2.0 协议的核心概念：
- 请求/响应格式
- 方法注册与调用
- 错误处理

协议规范: https://www.jsonrpc.org/specification

启动方式:
    python ch10/jsonrpc_server.py

服务器将在 http://localhost:8001 启动
"""

import json
import http.server
import socketserver
from http import HTTPStatus
from datetime import datetime
from typing import Any, Callable, Dict, List, Optional
from functools import wraps


# ============ JSON-RPC 2.0 数据模型 ============

class JsonRpcError(Exception):
    """JSON-RPC 错误"""
    def __init__(self, code: int, message: str, data: Any = None):
        self.code = code
        self.message = message
        self.data = data
        super().__init__(message)


# 标准错误码
class JsonRpcErrorCode:
    PARSE_ERROR = -32700          # 解析错误
    INVALID_REQUEST = -32600      # 无效请求
    METHOD_NOT_FOUND = -32601     # 方法未找到
    INVALID_PARAMS = -32602       # 无效参数
    INTERNAL_ERROR = -32603       # 内部错误
    SERVER_ERROR = -32000         # 服务器错误


# ============ 方法注册器 ============

class MethodRegistry:
    """JSON-RPC 方法注册器"""

    def __init__(self):
        self._methods: Dict[str, Callable] = {}

    def register(self, name: str = None):
        """注册方法的装饰器"""
        def decorator(func: Callable) -> Callable:
            method_name = name or func.__name__
            self._methods[method_name] = func
            return func
        return decorator

    def get(self, name: str) -> Optional[Callable]:
        """获取方法"""
        return self._methods.get(name)

    def list_methods(self) -> List[str]:
        """列出所有方法"""
        return list(self._methods.keys())


# 创建全局注册器
registry = MethodRegistry()


# ============ 业务方法定义 ============

@registry.register()
def add(a: float, b: float) -> float:
    """加法运算"""
    return a + b


@registry.register()
def subtract(a: float, b: float) -> float:
    """减法运算"""
    return a - b


@registry.register()
def multiply(a: float, b: float) -> float:
    """乘法运算"""
    return a * b


@registry.register()
def divide(a: float, b: float) -> float:
    """除法运算"""
    if b == 0:
        raise JsonRpcError(
            JsonRpcErrorCode.INVALID_PARAMS,
            "除数不能为零"
        )
    return a / b


@registry.register()
def greet(name: str, greeting: str = "你好") -> str:
    """问候语生成"""
    return f"{greeting}，{name}！"


@registry.register()
def get_info() -> dict:
    """获取服务器信息"""
    return {
        "server": "JSON-RPC 2.0 Demo Server",
        "version": "1.0.0",
        "time": datetime.now().isoformat(),
        "methods": registry.list_methods(),
    }


@registry.register()
def calculate(expression: str) -> float:
    """安全计算表达式"""
    allowed_names = {
        "abs": abs, "round": round, "max": max, "min": min,
        "pow": pow, "sum": sum,
    }
    try:
        result = eval(expression, {"__builtins__": {}}, allowed_names)
        return result
    except Exception as e:
        raise JsonRpcError(
            JsonRpcErrorCode.INVALID_PARAMS,
            f"表达式计算错误: {str(e)}"
        )


@registry.register("system.listMethods")
def list_methods() -> List[str]:
    """系统方法：列出所有可用方法"""
    return registry.list_methods()


# ============ JSON-RPC 请求处理器 ============

class JsonRpcHandler:
    """JSON-RPC 2.0 请求处理器"""

    def handle_request(self, request_data: dict) -> Optional[dict]:
        """
        处理单个 JSON-RPC 请求

        Args:
            request_data: 解析后的 JSON-RPC 请求对象

        Returns:
            响应对象（通知请求返回 None）
        """
        # 验证 JSON-RPC 版本
        if request_data.get("jsonrpc") != "2.0":
            return self._create_error_response(
                None, JsonRpcErrorCode.INVALID_REQUEST, "无效的 JSON-RPC 版本"
            )

        # 获取请求 ID（通知请求没有 id）
        request_id = request_data.get("id")

        # 获取方法名
        method_name = request_data.get("method")
        if not method_name or not isinstance(method_name, str):
            return self._create_error_response(
                request_id, JsonRpcErrorCode.INVALID_REQUEST, "缺少或无效的方法名"
            )

        # 获取参数
        params = request_data.get("params", [])

        # 查找方法
        method = registry.get(method_name)
        if method is None:
            return self._create_error_response(
                request_id, JsonRpcErrorCode.METHOD_NOT_FOUND,
                f"方法 '{method_name}' 未找到"
            )

        # 调用方法
        try:
            if isinstance(params, dict):
                result = method(**params)
            elif isinstance(params, list):
                result = method(*params)
            else:
                result = method()

            # 通知请求不需要返回结果
            if request_id is None:
                return None

            return self._create_success_response(request_id, result)

        except JsonRpcError as e:
            return self._create_error_response(request_id, e.code, e.message, e.data)
        except TypeError as e:
            return self._create_error_response(
                request_id, JsonRpcErrorCode.INVALID_PARAMS,
                f"参数错误: {str(e)}"
            )
        except Exception as e:
            return self._create_error_response(
                request_id, JsonRpcErrorCode.INTERNAL_ERROR,
                f"内部错误: {str(e)}"
            )

    def _create_success_response(self, request_id: Any, result: Any) -> dict:
        """创建成功响应"""
        return {
            "jsonrpc": "2.0",
            "result": result,
            "id": request_id,
        }

    def _create_error_response(
        self, request_id: Any, code: int, message: str, data: Any = None
    ) -> dict:
        """创建错误响应"""
        error = {
            "code": code,
            "message": message,
        }
        if data is not None:
            error["data"] = data

        return {
            "jsonrpc": "2.0",
            "error": error,
            "id": request_id,
        }


# ============ HTTP 服务器 ============

class JsonRpcHTTPHandler(http.server.BaseHTTPRequestHandler):
    """JSON-RPC HTTP 请求处理器"""

    rpc_handler = JsonRpcHandler()

    def do_POST(self):
        """处理 POST 请求"""
        # 只处理 /rpc 路径
        if self.path != "/rpc":
            self._send_error(HTTPStatus.NOT_FOUND, "路径未找到，请使用 /rpc")
            return

        # 读取请求体
        content_length = int(self.headers.get("Content-Length", 0))
        if content_length == 0:
            self._send_error(HTTPStatus.BAD_REQUEST, "请求体为空")
            return

        body = self.rfile.read(content_length)

        # 解析 JSON
        try:
            request_data = json.loads(body.decode("utf-8"))
        except json.JSONDecodeError as e:
            self._send_rpc_error(JsonRpcErrorCode.PARSE_ERROR, f"JSON 解析错误: {str(e)}")
            return

        # 处理批量请求
        if isinstance(request_data, list):
            responses = []
            for req in request_data:
                response = self.rpc_handler.handle_request(req)
                if response is not None:
                    responses.append(response)

            if responses:
                self._send_response(HTTPStatus.OK, responses)
            else:
                self._send_response(HTTPStatus.NO_CONTENT, None)
            return

        # 处理单个请求
        response = self.rpc_handler.handle_request(request_data)

        # 通知请求返回 204 No Content
        if response is None:
            self._send_response(HTTPStatus.NO_CONTENT, None)
            return

        self._send_response(HTTPStatus.OK, response)

    def do_GET(self):
        """处理 GET 请求 - 返回服务器信息"""
        info = {
            "server": "JSON-RPC 2.0 Demo Server",
            "version": "1.0.0",
            "rpc_endpoint": "/rpc",
            "methods": registry.list_methods(),
        }
        self._send_response(HTTPStatus.OK, info)

    def _send_response(self, status: int, data: Any):
        """发送 JSON 响应"""
        self.send_response(status)
        self.send_header("Content-Type", "application/json")
        self.send_header("Access-Control-Allow-Origin", "*")
        self.end_headers()

        if data is not None:
            self.wfile.write(json.dumps(data, ensure_ascii=False).encode("utf-8"))

    def _send_error(self, status: int, message: str):
        """发送 HTTP 错误"""
        self.send_response(status)
        self.send_header("Content-Type", "application/json")
        self.end_headers()
        self.wfile.write(json.dumps({"error": message}).encode("utf-8"))

    def _send_rpc_error(self, code: int, message: str):
        """发送 JSON-RPC 错误"""
        error_response = {
            "jsonrpc": "2.0",
            "error": {"code": code, "message": message},
            "id": None,
        }
        self._send_response(HTTPStatus.OK, error_response)

    def log_message(self, format, *args):
        """自定义日志格式"""
        print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] {args[0]}")


# ============ 启动入口 ============

if __name__ == "__main__":
    PORT = 8001

    print(f"JSON-RPC 2.0 Server 启动中...")
    print(f"  - 监听地址: http://localhost:{PORT}")
    print(f"  - RPC 端点: http://localhost:{PORT}/rpc")
    print(f"  - 可用方法: {', '.join(registry.list_methods())}")
    print(f"\n按 Ctrl+C 停止服务器\n")

    with socketserver.TCPServer(("", PORT), JsonRpcHTTPHandler) as httpd:
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\n服务器已停止")
