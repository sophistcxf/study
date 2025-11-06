import asyncio
import websockets
from urllib.parse import urlparse, parse_qs

# 存储活跃的websocket连接
websocket_clients = set()
# 存储tcp服务器
tcp_servers = {}

async def handle_tcp_client(reader, writer):
    """处理tcp客户端连接"""
    addr = writer.get_extra_info('peername')
    message = f"tcp client connected: {addr}"
    await broadcast_message(message)

    try:
        while True:
            data = await reader.read(1024)
            if not data:
                break
            message = f"received from {addr}: {data.decode()}"
            await broadcast_message(message)
    except Exception as e:
        await broadcast_message(f"tcp client error: {str(e)}")
    finally:
        writer.close()
        await writer.wait_closed()
        await broadcast_message(f"tcp client disconnected: {addr}")

async def broadcast_message(message):
    """广播消息给所有连接的websocket客户端"""
    if websocket_clients:
        # 创建发送任务列表
        tasks = [client.send(message) for client in websocket_clients]
        # 并发执行所有发送任务，忽略异常
        await asyncio.gather(*tasks, return_exceptions=True)

async def handle_websocket(websocket):
    """处理websocket连接"""
    # 添加客户端到集合
    websocket_clients.add(websocket)

    tcp_server = None

    try:
        # 获取路径和查询参数 (使用正确的属性)
        path = websocket.request.path
        # 解析url参数获取端口号
        parsed_url = urlparse(path)
        params = parse_qs(parsed_url.query)
        port_values = params.get('port', [])

        if not port_values:
            await websocket.send("error: no port specified")
            return

        port = int(port_values[0])

        if port < 1 or port > 65535:
            await websocket.send("error: invalid port number")
            return

        # 创建tcp服务器
        try:
            tcp_server = await asyncio.start_server(handle_tcp_client, 'localhost', port)
            tcp_servers[websocket] = tcp_server
            await websocket.send(f"successfully started listening on port {port}")

            # 等待websocket连接断开
            await websocket.wait_closed()

        except Exception as e:
            await websocket.send(f"failed to start server on port {port}: {str(e)}")

    except ValueError:
        await websocket.send("error: invalid port format")
    except Exception as e:
        await websocket.send(f"error: {str(e)}")
    finally:
        # 清理资源
        websocket_clients.discard(websocket)
        if websocket in tcp_servers:
            tcp_servers[websocket].close()
            await tcp_servers[websocket].wait_closed()
            del tcp_servers[websocket]

async def main():
    """主函数"""
    # 启动websocket服务器
    async with websockets.serve(handle_websocket, "localhost", 3000):
        print("websocket server starting on ws://localhost:3000")
        print("press ctrl+c to stop the server")
        await asyncio.Future()  # 运行 forever

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\nserver stopped")
