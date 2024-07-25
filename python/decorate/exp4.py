class MyDecorator:
    def __init__(self, func):
        self.func = func
    
    def __call__(self, *args, **kwargs):
        print("Something is happening before the function is called.")
        result = self.func(*args, **kwargs)
        print("Something is happening after the function is called.")
        return result

    def original_func(self, *args, **kwargs):
        return self.func(*args, **kwargs)

@MyDecorator
def say_hello():
    print("Hello!")

# 调用装饰后的函数
say_hello()