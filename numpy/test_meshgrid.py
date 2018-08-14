#!-*- coding=utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt

def test1():
    nx, ny = (3, 2)
    x = np.linspace(0, 1, nx)
    print "x:"
    print x
    y = np.linspace(0, 1, ny)
    print "y:"
    print y
    # xv, yv 都是 2*3，每个 ij 元素一一对应，形成的 12 个点画了一个网格
    xv, yv = np.meshgrid(x, y)
    print "xv:"
    print xv
    print "yv:"
    print yv

    print "np.vstack([xv,yv]):"
    print np.vstack([xv.ravel(), yv.ravel()])

def test2():
    x = np.arange(-5, 5, 0.1)
    y = np.arange(-5, 5, 0.1)
    xx, yy = np.meshgrid(x, y, sparse=True)
    z = np.sin(xx**2 + yy**2) / (xx**2 + yy**2)
    plt.contourf(x,y,z)
    plt.show()

def test3():
    nx, ny, nz = (3,2,1)
    x = np.linspace(0, 1, nx)
    y = np.linspace(0, 1, ny)
    z = np.linspace(0, 1, nz)
    xx, yy, zz = np.meshgrid(x, y, z)
    print "xx:"
    print xx
    print "yy:"
    print yy
    print "zz:"
    print zz

test1()
#test2()
#test3()
