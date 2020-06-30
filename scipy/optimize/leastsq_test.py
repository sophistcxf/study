# coding=utf-8

import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
from scipy.optimize import leastsq

'''
leastsq 最小化目标函数，所有方程的平方和
'''
 

def test1():
    xi = np.array([160,165,158,172,159,176,160,162,171])
    yi = np.array([58,63,57,65,62,66,58,59,62])
     
    def func(p,x):
        k,b=p
        return k*x+b
     
    def error(p,x,y):
        return func(p,x)-y
     
    p0 = [1,20]
     
    para = leastsq(error, p0, args=(xi,yi), full_output=True)

    print 'leastsq result ', para
     
    k,b = para[0]
    print("k=",k,"b=",b)
     
     
    plt.figure(figsize=(8,6))
    plt.scatter(xi,yi,color="green",label="sample_datas",linewidth=2) 
     
    x=np.linspace(150,190,100)
    y=k*x+b
    plt.plot(x,y,color="red",label="fitting_curve",linewidth=2) 
    plt.legend()
    #plt.show()

test1()
