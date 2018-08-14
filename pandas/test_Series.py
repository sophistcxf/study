#-*- coding=utf-8 -*-

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def test_construct():
    ndarray = np.random.randn(5)
    s = pd.Series(ndarray, index=['a','b','a','d','e'])
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

    s['a'] = 10
    print s
    print d

    # value会变成NaN
    s = pd.Series(d, index=['a1','b1','c1','d1','e1'])
    print s

    d = ['a', 'b', 'c', 'd', 'e']
    s = pd.Series(d)
    print s

    # 如果用二维数据构造Series呢？
    # 以第一维axis来分
    d = [['a','b','c'], ['a1','b1', 'c1']]
    s = pd.Series(d)
    print s

def test_attributes():
    d = [['a','b','c'], ['a1','b1', 'c1'], ['a2','b2','c2']]
    s = pd.Series(d)
    print s
    
    print s.T
    print s.asobject
    print s.axes
    # type是buffer
    print type(s.data)
    print s.iat
    print s.index
    print s.ix
    print s.loc
    print s.ndim
    print s.shape
    print s.size
    print s.strides
    print s.values

def test_methods():
    d = [['1','2','3'], ['-1','-2', '-3'], ['4','5','6']]
    s = pd.Series(d, index=['a','b','c'])
    print s

    # 当数据是中element不是scalar时，会报错
    d1 = [-1, -2, 1, 2]
    s1 = pd.Series(d1)
    print s1
    print s1.abs()

    print s.add_prefix("prefix_")
    print s.agg(['min', 'max'], 0)
    # at按索引的值，iat按索引的位置，因而iat只能是数值
    print s.at['b']
    print s.iat[1]
    # iloc的参数可以是integer如5，可以是list如[0,2]，可以是slice如1:3，可以是boolean array
    print s.iloc[1:3]

def test_combine():
    d1 = ['a','b','c']
    d2 = ['e','f','g','h']
    s1 = pd.Series(d1)
    s2 = pd.Series(d2)
    print s1.append(s2)

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
#test_attributes()
#test_methods()
#test_combine()
