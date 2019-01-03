#! -*- coding=utf-8 -*-
import pandas as pd
import numpy as np

def test1():
    df = pd.DataFrame({'a': [1, 1, 3, 2, 5, 6], 'b': [1, 1, 2, 3, 2, 8], 'c': [1, 4, 9, np.nan, 25, 36], \
                        'd':[3, 4, 1, 2, 5, 6]})
    print df
    # df.groupby 返回的是 pandas.core.groupby.DataFrameGroupBy 对象
    gb = df.groupby(['b'])
    print type(gb)
    print gb
    print "groupby size"
    # gb.size() return Series
    size = gb.size()
    print size

    #gb.hist()
    print "groupby describe"
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

    print "groupby sum"
    print gb.sum()

    gb = df.groupby(['a', 'b'])
    s = gb['d'].agg('sum')
    print type(s)
    print s

    # 遍历 pandas.core.groupby.DataFrameGroupBy
    # i 是一个拥有 2 个元素的 tuple
    print "iterate pandas.core.groupby.DataFrameGroupBy"
    for i in df.groupby(['a', 'b']):
        print type(i)
        print len(i)
        # i[0] 是一个 tuple，i[1] 是一个 pandas.core.frame.DataFrame
        print type(i[0])
        print type(i[1])

    print "iterate pandas.core.groupby.DataFrameGroupBy group one"
    for i in df.groupby(['a']):
        print type(i)
        print len(i)
        # i[0] 是一个 int，i[1] 是一个 pandas.core.frame.DataFrame，由此可见，i[0] 会随着 group by 的
        # key 值的多少变化
        print type(i[0])
        print type(i[1])

if __name__ == "__main__":
    test1()
