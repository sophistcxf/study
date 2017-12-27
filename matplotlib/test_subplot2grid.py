#!-*- coding=utf-8 -*-
from matplotlib import pyplot as plt
import numpy as np

def test1():
    T = np.linspace(-np.pi, np.pi, 1024)
    grid_size = (4, 2)
    plt.subplot2grid(grid_size, (0, 0), rowspan = 3, colspan = 1)
    plt.plot(np.sin(2 * T), np.cos(0.5 * T), c = 'k')
    plt.subplot2grid(grid_size, (0, 1), rowspan = 3, colspan = 1)
    plt.plot(np.cos(3 * T), np.sin(T), c = 'k')
    plt.subplot2grid(grid_size, (3, 0), rowspan=1, colspan=3)
    plt.plot(np.cos(5 * T), np.sin(7 * T), c= 'k')
    # This command asks matplotlib to pack all the fgures so that none of them overlap each other
    plt.tight_layout()
    plt.show()

def get_radius(T, params):
    m, n_1, n_2, n_3 = params
    U = (m * T) / 4
    return (np.fabs(np.cos(U)) ** n_2 + np.fabs(np.sin(U)) ** n_3) ** (-1. / n_1)

def test2():
    grid_size = (3,4)
    T = np.linspace(0, 2 * np.pi, 1024)
    for i in range(grid_size[0]):
        for j in range(grid_size[1]):
            params = np.random.random_integers(1, 20, size = 4)
            R = get_radius(T, params)

            axes = plt.subplot2grid(grid_size, (i, j), rowspan=1, colspan=1)
            # 下面两句去掉坐标轴的标
            axes.get_xaxis().set_visible(False)
            axes.get_yaxis().set_visible(False)
            plt.plot(R * np.cos(T), R * np.sin(T), c = 'k')
            plt.title('%d, %d, %d, %d' % tuple(params), fontsize = 'small')
            
    # 若要给整个graphic一个标题，则调用suptitle，superior title
    plt.suptitle("Suptitle")
    # 在此例中，不调tight_layout的话3*4个sub plot不会充满整个graphic，两边留有空间
    plt.tight_layout()
    plt.show()

def test3():
    T = np.linspace(-np.pi, np.pi, 1024)
    fig, (ax0, ax1) = plt.subplots(ncols =2)
    # ax0是什么类型，为什么能调用plot
    ax0.plot(np.sin(2 * T), np.cos(0.5 * T), c = 'k')
    ax1.plot(np.cos(3 * T), np.sin(T), c = 'k')
    plt.show()

test3()
