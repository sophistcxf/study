def wrapper(func):
    print("wrapper ")
    func()

@wrapper
def say_hello():
    print("hello")
