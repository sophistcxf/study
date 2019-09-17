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

    d = {'a':1, 'b':2, 'c':3, 'd':4, 'e':5, 'f':0}
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
    d = [['a','b','c'], ['a1','b1', 'c1'], ['a2','b2','c2'], ['a3', 'b3', 'c3']]
    s = pd.Series(d)
    print s
    
    print s.T
    #print s.asobject   DEPRECATED
    print 's.axes: ', s.axes
    # type是buffer
    #print type(s.data) DEPRECATED
    print 's.iat: ', s.iat
    print 's.iloc: ', s.iloc
    print 's.index: ', s.index
    print 's.ix: ', s.ix
    print 's.loc: ', s.loc
    print 's.ndim: ', s.ndim
    print 's.shape: ', s.shape
    print 's.size: ', s.size
    #print s.strides    DEPRECATED
    print s.values
    print 's.name: ', s.name
    print 's.real: ', s.real

    '''
    is_unique 这个属性很诡异
    print 's.is_unique: ', s.is_unique
    d = {'a':1, 'b':2, 'c':3, 'd':4, 'e':5, 'f':0}
    s = pd.Series(d)
    print s
    print 'type(s.is_unique): ', type(s.is_unique)
    print 's.is_unique: ', s.is_unique
    '''

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
    d1 = [0, 4,10]
    d2 = [1, 2, 9, 11]
    s1 = pd.Series(d1)
    s2 = pd.Series(d2)
    print s1
    print s2
    s3 = s1.combine(s2, func=lambda x1, x2: x1 if x1 > x2 else x2, fill_value=20)
    print s3

def test_combine_first():
    d1 = [0, 4,10]
    d2 = [1, 2, 9, 11]
    s1 = pd.Series(d1)
    s2 = pd.Series(d2)
    print s1
    print s2
    s3 = s1.combine_first(s2)
    print s3

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

def test_value_counts():
    d = {'a':1, 'b':2, 'c':3, 'd':2, 'e':5, 'f':10, 'g':1, 'h':1}
    s = pd.Series(d)
    print s
    print 'series value_counts type: ', type(s.value_counts())
    print s.value_counts() 
    print s.value_counts(sort=True, ascending=True)
    print s.value_counts(normalize=True)

def test_transpose():
    d = {'a':1, 'b':2, 'c':3, 'd':2, 'e':5, 'f':10, 'g':1, 'h':1}
    s = pd.Series(d)
    print s
    print s.transpose()

def test_nunique():
    d = {'a':1, 'b':2, 'c':3, 'd':2, 'e':5, 'f':10, 'g':1, 'h':1}
    s = pd.Series(d)
    print s
    print s.nunique()

def test_item():
    '''
    直接遍历 Series，遍历的是 value
    '''
    d = {'a':1, 'b':2, 'c':3, 'd':2, 'e':5, 'f':10, 'g':1, 'h':1}
    s = pd.Series(d)
    print s
    print s.item()

def test_get():
    d = {'a':1, 'b':2, 'c':3, 'd':2, 'e':5, 'f':10, 'g':1, 'h':1}
    s = pd.Series(d)
    print s.get('a')
    print s.get(4)

def test_sort_index():
    '''
    按 index 或 value 排序
    '''
    d = {'a':1, 'b':2, 'c':3, 'd':2, 'e':5, 'f':10, 'g':1, 'h':1}
    s = pd.Series(d)
    # 按 value 排序
    s_sort_v = s.sort_values()
    print s_sort_v
    # 按 index 排序
    s_sort_i = s_sort_v.sort_index()
    print s_sort_i

def test_keys():
    d = {'a':1, 'b':2, 'c':3, 'd':2, 'e':5, 'f':10, 'g':1, 'h':1}
    s = pd.Series(d)
    keys = s.keys()
    print keys
    print list(keys)
    values = s.values
    print values

def test_items():
    d = {'a':1, 'b':2, 'c':3, 'd':2, 'e':5, 'f':10, 'g':1, 'h':1}
    s = pd.Series(d)
    for index, value in s.items():
        print index, value

def test_iter():
    '''
    遍历一个 Series
    '''
    d = {'a':1, 'b':2, 'c':3, 'd':2, 'e':5, 'f':10, 'g':1, 'h':1}
    s = pd.Series(d)
    for i in s:
        print type(i)
        print i

def to_frame():
    d = {'a':1, 'b':2, 'c':3, 'd':2, 'e':5, 'f':10, 'g':1, 'h':1}
    s = pd.Series(d)
    df = s.to_frame()
    print df

def test_unique():
    '''
    unique 的返回值是 numpy.ndarray 类型
    '''
    d = {'a':1, 'b':2, 'c':3, 'd':2, 'e':5, 'f':10, 'g':1, 'h':1}
    s = pd.Series(d)
    u = s.unique()
    print u
    print type(u)

def test_operator():
    '''
    测试 series 可以与哪些操作符混用
    '''
    d = {'a':1, 'b':2, 'c':3, 'd':2, 'e':5, 'f':10, 'g':1, 'h':0}
    s = pd.Series(d)

    print s

    # t1 是一个 series
    s_t = s>1
    print type(s_t)
    print s_t.describe()

    s_t = s-1
    print type(s_t)
    print s_t

    # 返回是一个 bool 型 false
    s_t = 1 in s
    print type(s_t)
    print s_t

    # 返回 bool 型 true
    s_t = 'a' in s
    print type(s_t)
    print s_t

    s_t = abs(s)
    print type(s_t)
    print s_t

    #s_t = not s
    s_t = s_t.apply(lambda x: not x)
    print type(s_t)
    print s_t

def to_dict():
    '''
    Series 是否可以转为列名为 key 的 dict
    '''
    d = {'a':1, 'b':2, 'c':3, 'd':4, 'e':5, 'f':0}
    s = pd.Series(d)
    to_d = dict(s)
    print to_d

def test_cumsum():
    d = {'a':1, 'b':2, 'c':3, 'd':4, 'e':5, 'f':0}
    s = pd.Series(d)
    print s.cumsum()

    '''
    如果遇到 np.nan，则当前元素的 cumsum 是 NaN
    '''
    d = {'a':1, 'b':2, 'c':3, 'd':np.nan, 'e':5, 'f':0}
    s = pd.Series(d)
    print s.cumsum()

def test_cummax():
    '''
    cummax 得到当前行与前面行的最大值
    '''
    d = {'a':1, 'b':2, 'c':3, 'd':4, 'e':5, 'f':0}
    s = pd.Series(d)
    print s.cummax()

def test_cummin():
    '''
    cummax 得到当前行与前面行的最小值
    '''
    d = {'a':1, 'b':2, 'c':3, 'd':4, 'e':5, 'f':0}
    s = pd.Series(d)
    print s.cummin()

def test_cumprod():
    '''
    cumprod 得到累积的积
    '''
    d = {'a':1, 'b':2, 'c':3, 'd':4, 'e':5, 'f':0}
    s = pd.Series(d)
    print s.cumprod()

def test_max():
    '''
    测试 max
    '''
    d = {'a':1, 'b':2, 'c':3, 'd':4, 'e':5, 'f':0}
    s = pd.Series(d)
    print max(s) 

def select_by_index():
    '''
    按 index 筛选
    '''
    d = ['a', 'c', 'e', 'b', 'f', 'i', 'h']
    s = pd.Series(d)
    print s
    print 'type(s.index): ', type(s.index)
    select_index = s.index[s.index>3]
    print 'select index type: ', type(select_index)
    print 'loc type: ', type(s.loc[select_index])
    print s.loc[select_index]

#test_construct()
#test_plot()
#test_hist()
test_attributes()
#test_methods()
#test_combine()
#test_value_counts()
#test_transpose()
#test_nunique()
#test_item()
#test_combine_first()
#test_sort_index()
#test_keys()
#test_items()
#to_frame()
#test_unique()
#test_operator()
#to_dict()
#test_cumsum()
#test_cummax()
#test_cummin()
#test_cumprod()
#test_max()
#test_iter()
#test_get()
#select_by_index()
