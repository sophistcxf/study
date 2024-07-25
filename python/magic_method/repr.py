class MyClass:
    def __init__(self, name, value):
        self.name = name
        self.value = value

    def __str__(self):
        return f"str: MyClass(name={self.name!r}, value={self.value!r})"

    def __repr__(self):
        return f"repr: MyClass(name={self.name!r}, value={self.value!r})"

# 创建实例
obj = MyClass("example", 42)

# 打印对象，调用 __repr__ 方法
print(obj)  # 输出: MyClass(name='example', value=42)