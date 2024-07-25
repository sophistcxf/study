class A():
    def __rlshift__(self, other):
        print(other)

s = 'hello '
s << A()