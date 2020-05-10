# coding=utf-8

'''
实现 scipy.optimize 官网的一些 case
https://docs.scipy.org/doc/scipy/reference/tutorial/optimize.html
'''

from scipy.optimize import minimize
import numpy as np

xo = [1.3, 0.7, 0.8, 1.9, 1.2]
x0 = [1.3, 0.7, 0.8, 1.9, 1.2, 2.0, 3.0]

def rosen(x):
    return sum(100.0*(x[1:]-x[:-1]**2.0)**2.0 + (1-x[:-1])**2.0)

def rosen_der(x):
    '''
    rosen 函数的梯度
    '''
    xm = x[1:-1]
    xm_m1 = x[:-2]
    xm_p1 = x[2:]
    der = np.zeros_like(x)
    der[1:-1] = 200*(xm-xm_m1**2) - 400*(xm_p1 - xm**2)*xm - 2*(1-xm)
    der[0] = -400*x[0]*(x[1]-x[0]**2) - 2*(1-x[0])
    der[-1] = 200*(x[-1]-x[-2]**2)
    return der

def test_nelder_mead():
    x0 = [1.3, 0.7, 0.8, 1.9, 1.2]
    res = minimize(rosen, x0, method='Nelder-Mead', tol=1e-6)
    print res

def test_nelder_mead():
    x0 = [1.3, 0.7, 0.8, 1.9, 1.2]
    res = minimize(rosen, x0, method='powell', tol=1e-6)
    print res

def test_bfgs():
    res = minimize(rosen, x0, method='BFGS', jac=rosen_der,
               options={'disp': True})
    print res

test_nelder_mead()
test_bfgs()
