#!-*- coding=utf-8 -*-
import matplotlib.pyplot as plt
import numpy as np

def test1():
    z = np.array([[1,5,3],[2,4,5],[2,1,4]])
    print z
    #plt.contourf(z)
    plt.contour(z, levels=[0,1,2,3,4,5], colors=["r",'g','b','y','c',"k"])
    plt.show()

def test2():
    def f(x, y):
        return (1-x/2+x**5+y**3) * np.exp(-x**2-y**2)
    # x 和 y 定义了 256*256 的网格，z定义了网格上每个点的值
    n = 256
    x = np.linspace(-3, 3, n)
    y = np.linspace(-3, 3, n)
    X, Y = np.meshgrid(x, y)
    z = f(X,Y)
    print "X shape: ", X.shape
    print "Y shape: ", Y.shape
    print "Z shape: ", z.shape
    # contour 只画线，contourf 画出填充的画
    plt.contourf(X, Y, z)
    #plt.contourf(z)
    #plt.contour(X, Y, f(X,Y))
    plt.show()

def test3():
    def f(x, y):
        return (1-x/2+x**5+y**3) * np.exp(-x**2-y**2)
    # x 和 y 定义了 256*256 的网格，z定义了网格上每个点的值
    n = 256
    x = np.linspace(-3, 3, n)
    y = np.linspace(-3, 3, n)
    X, Y = np.meshgrid(x, y)
    z = f(X,Y)
    print "X shape: ", X.shape
    print "Y shape: ", Y.shape
    print "Z shape: ", z.shape
    # contour 只画线，contourf 画出填充的画
    # 与 test2 添加了 levels
    plt.contourf(X, Y, z, levels=[-0.5,0,0.5])
    #plt.contourf(z)
    #plt.contour(X, Y, f(X,Y))
    plt.show()

def test4():
    z = np.array([[3,3,3,3],[3,2,2,3],[3,2,2,3],[3,3,3,3]])
    print z
    #plt.contourf(z)
    plt.contour(z, levels=[1,1.5,2,2.5,2.7], colors=['r','r','b','y','g'])
    plt.show()

test1()
#test2()
#test3()
