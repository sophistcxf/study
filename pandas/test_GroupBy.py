#! -*- coding=utf-8 -*-
import pandas as pd
import numpy as np

def test1():
    df = pd.DataFrame({'a': [1, 2, 3, np.nan, 5, 6], 'b': [1, 1, 2, 3, 2, 8], 'c': [1, 4, 9, 16, 25, 36]})
    print df
    # df.groupby 返回的是 pandas.core.groupby.DataFrameGroupBy 对象
    gb = df.groupby(['b'])
    print type(gb)
    print gb
    print "groupby size"
    # gb.size() return Series
    size = gb.size()
    print size

    gb.hist()
    print gb.describe()

    # gb.count 并不是计算每个 group 有多少条记录，在每个 group 下其他元素有多少个有效的(非 np.nan)
    print "groupby count"
    print gb.count()

    print "groupby corr"
    print gb.corr()

    print "groupby min"
    print gb.min()

    print "groupby last"
    print gb.last()

    print "groupby max"
    print gb.max()

    print "groupby median"
    print gb.median()

    print gb.mean()

    print "groupby 0"
    print gb.nth(0)

if __name__ == "__main__":
    test1()
