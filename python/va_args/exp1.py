def add(*args):
    sum = 0
    for x in args:
        sum += x
    return sum

print(add(1,2,3))

def add2(base, multi=1, *args):
    sum = base
    for x in args:
        sum += x
    sum *= multi
    return sum

# 第2个参数3，会被解释为multi
print(add2(2, 3, 1, 2, 3))

def add3(base, *args, multi = 1):
    sum = base
    for x in args:
        sum += x
    sum *= multi
    return sum

# 没有设置multi
print(add3(2, 3, 1, 2, 3))


# 错误：positional argument follows keyword argument
# print(add3(2, multi=3, 1, 2, 3))

'''
**kwargs必须在*args后面
def add4(**kwargs, *args):
    pass
'''

def add5(*args, **kwargs):
    pass
