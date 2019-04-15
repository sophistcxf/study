#! -*- coding=utf-8 -*-
"""
===========
Simple Plot
===========

Create a simple plot.
"""

import matplotlib
import matplotlib.pyplot as plt
import numpy as np

def test():
    # Data for plotting
    t = np.arange(0.0, 2.0, 0.01)
    s = 1 + np.sin(2 * np.pi * t)

    fig, ax = plt.subplot()

    # l 是一个 list，只有一个元素
    l = ax.plot(t, s)
    print type(l),len(l)
    # l[0] 是一个 matplotlib.lines.Line2D
    print type(l[0])

    ax.set(xlabel='time (s)', ylabel='voltage (mV)',
           title='About as simple as it gets, folks')
    ax.grid()

    #fig.savefig("test.png")
    #plt.show()

def test_plot_return():
    """
    测试 plot 的返回值
    """
    t = np.arange(0.0, 2.0, 0.01)
    s = 1 + np.sin(2 * np.pi * t)

    plt.subplot(1, 2, 1)
    # l 是一个 list，只有一个元素
    l = plt.plot(t, s)
    print type(l),len(l)
    # l[0] 是一个 matplotlib.lines.Line2D
    print type(l[0])

    s2 = 1 + np.cos(2 * np.pi * t)
    plt.subplot(1, 2, 2)
    l = plt.plot(t, s, t, s2)
    print type(l),len(l)
    # l[0] 和 l[1] 是 matplotlib.lines.Line2D
    print type(l[0]),type(l[1])

    plt.show()

def test_category_line():
    """
    测试画折线时，x 坐标是 category，而不是数值
    """
    x = ['a', 'b', 'c', 'd', 'e', 'f', 'g']
    y = [1, 2, 3, 4, 3, 2, 1]
    plt.plot(x, y)
    plt.show()

if __name__ == "__main__":
    #test_plot_return()
    test_category_line()
