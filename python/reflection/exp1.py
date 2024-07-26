class TestClass:
    def say_hello(self, name):
        print("Hello, " + name)

if __name__ == "__main__":
    # 获取类和方法
    clazz = globals()["TestClass"]
    method = getattr(clazz(), "say_hello")

    # 调用方法
    method("World")
