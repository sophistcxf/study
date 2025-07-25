from fastmcp import FastMCP
# 初始化FastMCP server，创建一个MCP服务器实例
mcp = FastMCP("mcpserver")

# @MCP.tool()装饰器将这个函数注册为MCP工具
@mcp.tool()
def add(x: int, y: int) -> int:
    return x + y

if __name__ == "__main__":
    mcp.run(transport="http", port=5001)
