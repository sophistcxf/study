import asyncio

# 定义一个异步函数
async def my_coroutine():
    print("开始执行协程")
    await asyncio.sleep(20)  # 模拟 I/O 操作，例如网络请求或文件读写
    print("协程执行完毕")

# 定义一个主函数
async def main():
    # 创建协程，但是并不执行
    #task = asyncio.create_task(my_coroutine())
    tasks = [my_coroutine(), my_coroutine()]
    print("等待协程完成")
    # 执行协程，且等待
    await asyncio.gather(*tasks)
    print("协程已完成")

# 运行事件循环
asyncio.run(main())
