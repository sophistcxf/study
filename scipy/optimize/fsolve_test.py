from scipy.optimize import fsolve

def test1():
    def func(x, a, b, c):
        return a * x ** 2 + b * x + c

    x = 0
    res = fsolve(func, x, args=(1, 2, -1))
    print res

    '''
    x^2 + x - 6 = 0
    只找到一个根 2
    '''
    x = 0
    print fsolve(func, x, args=(1, 1, -6))
    print fsolve(func, x, args=(1, 1, -6), full_output=True)

test1()
