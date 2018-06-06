#-*- coding=utf-8 -*-

import numpy as np
import pandas as pd

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

test_construct()
