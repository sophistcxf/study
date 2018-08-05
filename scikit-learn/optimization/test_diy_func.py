from scipy.optimize import minimize, rosen, rosen_der

def test1():
    fun = lambda x: (x[0] - 1)**2 + (x[1] - 2.5)**2

    cons = ({'type': 'ineq', 'fun': lambda x:  x[0] - 2 * x[1] + 2},
            {'type': 'ineq', 'fun': lambda x: -x[0] - 2 * x[1] + 6},
            {'type': 'ineq', 'fun': lambda x: -x[0] + 2 * x[1] + 2})

    bnds = ((0, None), (0, None))

    init = [2, 0]
    res = minimize(fun, init, method='SLSQP', bounds=bnds,constraints=cons)

    print res.x

test1()
