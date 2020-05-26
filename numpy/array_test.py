# coding=utf-8

'''
测试numpy的array
'''

import numpy as np

def test1():
    arr = np.array([[1,2], [11, 22], [111,222]])

    print arr

    # 获取第一行，第一列
    print arr[1,1]

    # 获取所有行，第0列
    print arr[:, 0]

    # 获取所有行，第1列
    print arr[:, 1]

    # 获取所有列，第0行
    print arr[0, :]

    # 按行将多维改为一维
    print arr.ravel()

test1()
