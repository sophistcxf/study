class A():
    def __getattr__(self, name):
        print(type(name))
        print(name)

a = A()
a.n1