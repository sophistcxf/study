from scipy.optimize import minimize_scalar

def test1():
    def f(x):
        return (x - 2) * x * (x + 2)**2

    print "Brent..."

    res = minimize_scalar(f)
    print res

    print

    print "Bounded..."
    res = minimize_scalar(f, bounds=(-3, -1), method='bounded')
    print res

def test2():
    def f(x, a, b):
        return a * (x-2)*x * (b*x+2)**2

    res = minimize_scalar(f, args=(2, 3))
    print res

test2()
