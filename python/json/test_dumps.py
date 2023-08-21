import json

class A():
    def __init__(self):
        self.a = 10
        self.b = 'zhangsan'
        self.segs = ['a', 'b']

a = A()
print(a.__dict__)

b = ['a', 'b', 'c']
print(json.dumps(b))

print(json.dumps(a, default=lambda o: o.__dict__))
