#-*- coding=utf-8 -*-

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def test_construct():
    ndarray = np.random.randn(5)
    s = pd.Series(ndarray, index=['a','b','c','d','e'])
    print s
    # 可以像dict一样取索引
    print s['a']
    print 'd' in s
    print 'f' in s
    # 如果有'f'返回s['f']，没有返回9999
    print s.get('f', 9999)

    d = {'a':1, 'b':2, 'c':3, 'd':4, 'e':5}
    s = pd.Series(d)
    print s

    # value会变成NaN
    s = pd.Series(d, index=['a1','b1','c1','d1','e1'])
    print s

def test_plot():
    d = {'a':1, 'b':2, 'c':3, 'd':4, 'e':5}
    s = pd.Series(d)
    s.plot()
    plt.show()

def test_hist():
    # 统计各个值的出现次数
    d = {'a':1, 'b':2, 'c':3, 'd':2, 'e':5, 'f':10, 'g':1, 'h':1}
    s = pd.Series(d)
    s.hist()
    plt.show()

    '''
    df3 = pd.DataFrame({'a': np.random.randn(1000) + 1, 'b': np.random.randn(1000)}, columns=['a', 'b'])
    df3['a'].hist();
    print df3['a']
    plt.show()
    '''

#test_construct()
#test_plot()
test_hist()

