@staticmethod
def say_hello():
    print('hello')


def say_hello2():
    print('hello2')

say_hello2 = staticmethod(say_hello2)

say_hello2()