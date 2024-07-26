a = 10

def say_hello():
    print("hello")


class Person():
    def __init__(self):
        self.name = ""

g = globals();

# g是一个dict
print(type(g))

print(g)
