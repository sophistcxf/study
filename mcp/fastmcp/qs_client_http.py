import asyncio
from fastmcp import Client

#client = Client("qs_server.py")
client = Client("http://127.0.0.1:5001/mcp")

async def call_tool(name: str):
    async with client:
        result = await client.call_tool("greet", {"name": name})
        print(result)

asyncio.run(call_tool("Ford"))
