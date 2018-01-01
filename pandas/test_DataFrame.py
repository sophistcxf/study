#!-*- coding=utf-8 -*-
import pandas as pd
import numpy as np

def constructDataFrame():
    # 通过numpy ndarray构造
    #通过dict构造，每一列的元素个数必须一样，否则抛异常ValueError('arrays must all be same length')
    #index的大小要与行数一致，否则抛异常ValueError: Shape of passed values is (4, 2), indices imply (4, 1)，这个提示
    #信息很奇怪，先列后行
    d = {'col1':[1,2], 'col2':[3,4], 'col3':[5,6], 'col4':[7,8]}
    df = pd.DataFrame(data=d, index=["row1", "row2"])

    #如果不指定列名或行名，就由np.arange(n)给默认值
    df2 = pd.DataFrame(np.random.randint(low=0, high=10, size=(3,5)))

    df2 = pd.DataFrame(np.random.randint(low=0, high=10, size=(3,5)),
                    columns=['a','b','c','d','e'])

constructDataFrame()
