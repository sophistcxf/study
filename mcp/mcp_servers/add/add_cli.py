import asyncio
from fastmcp import Client

client = Client("http://127.0.0.1:5001/mcp")

async def call_tool(x: int, y: int):
    async with client:
        result = await client.call_tool("add", {"x": x, "y": y})
        print(result)

asyncio.run(call_tool(10, 21))
