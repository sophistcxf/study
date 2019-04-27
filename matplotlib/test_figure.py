#! -*- coding=utf-8 -*-
import matplotlib.pyplot as plt
import matplotlib

def test_add_subplot():
    fig = matplotlib.figure.Figure() 
    fig.show()

def test_pyplot():
    fig = plt.figure()
    help(fig)

def test_figure_member():
    '''
    测试 Figure 的成员变量和函数
    '''
    fig = plt.figure()

    # 给 figure 添加一个 title，注意不是给 window 添加
    # 返回一个 matplotlib.text.Text
    text = fig.suptitle('This is a figure')

if __name__ == "__main__":
    #test_add_subplot()
    #test_pyplot()
    test_figure_member()
    plt.show()
