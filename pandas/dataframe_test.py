#! -*- coding=utf-8 -*-
import pandas as pd
import numpy as np

def constructDataFrame():
    '''
    构造 DataFrame
    * 通过numpy ndarray构造
    * 通过dict构造，每一列的元素个数必须一样，否则抛异常ValueError('arrays must all be same length')
    index的大小要与行数一致，否则抛异常ValueError: Shape of passed values is (4, 2), indices imply (4, 1)，这个提示
    信息很奇怪，先列后行
    '''
    d = {'col1':[1,2], 'col2':[3,4], 'col3':[5,6], 'col4':["str",8]}
    df = pd.DataFrame(data=d, index=["row1", "row2"])

    print df
    print df.dtypes

    #如果不指定列名或行名，就由np.arange(n)给默认值
    df2 = pd.DataFrame(np.random.randint(low=0, high=10, size=(3,5)))

    df3 = pd.DataFrame(np.random.randint(low=0, high=10, size=(3,5)),
                    columns=['a','b','c','d','e'])

    print df2.join(df3)

def dml_DataFrame():
    '''
    对 DataFrame 进行数据管理
    '''
    print 'DataFrame的样子'
    df = pd.DataFrame({'a': [1, 2, 3, 4, 5, 6],
                       'b': [1, 1, 2, 3, 5, 8],
                       'c': [1, 4, 9, 16, 25, 36],
                       'd': [11,22,33,44,55,66]})
    print df

    # 取出 index
    print '取出 index'
    print 'index 的类型: ', type(df.index)
    print df.index

    # 取出 column names
    print '取出 column names'
    print 'column name 的类型: ', type(df.columns)
    print df.columns

    # 转置行/列
    print '转置行/列'
    print df.T

    # 取出 'a'
    print type(df[['a']])   # 这样取出是 DataFrame
    print type(df['a'])     # 这样取出是 Series
    # 取出 'a','b'
    print type(df[['a', 'b']])  # 这样取出是 DataFrame

    # 取出第2,3列
    print '取出第2,3列'
    print df.iloc[:,[1,2]]
    print df.iloc[:, lambda df:[1,2]]

    # 取出列 b 的第 3 行元素
    print '取出列 b 的第 3 行元素'
    print df.at[3, 'b']
    print 'at 不仅可以访问，还可以修改'
    df.at[3, 'b'] = 4
    print df.at[3, 'b']

    # df 的中括号里的值其实是 bool 型的 pandas.Series
    # 取出a>5的行
    print '取出a>5的行'
    print df[df.a>5]
    # 取出 a>=3 且 b < 5 的行
    print '取出 a>=3 且 b < 5 的行'
    cond = (df.a>=3) & (df.b<5)
    print 'cond 的类型: ', type(cond)
    print 'cond 的值: \n', cond
    print df[cond]

    # 取出 a<3 || b>=5 的行，即上一个条件的反
    print '取出 a<3 || b>=5 的行，即上一个条件的反'
    print df[cond.apply(lambda x: not x)]

    # 修改列a的值
    print '对列a乘100后，添加为列a_multi100'
    df['a_multi100'] = df['a'] * 10
    df['a_multi100'] = df['a_multi100'].apply(lambda x: x*10)

    print df

def write_to_file(df):
    df.to_csv('df.csv', sep=',', header=True, index=False)
    df.to_json('df.json')
    df.to_html('df.html')

def df2array(df):
    arr = df.values 
    print type(arr)
    return arr

def join():
    df1 = pd.DataFrame(np.random.randint(low=0, high=10, size=(3,5)))

    df2 = pd.DataFrame(np.random.randint(low=0, high=10, size=(3,5)),
                    columns=['a','b','c','d','e'])

    print df1
    print df2
    print df1.join(df2)

def merge():
    pass

def concat():
    pass

def groupby():
    d = {'color':['red','blue','red','blue','black','red'], 'count':[1,2,3,3,2,1], 'age':[20,25,25,20,30,30]}
    df = pd.DataFrame(data=d)
    for key, grp in df.groupby(["color"]):
        '''
        当 groupby 只按一列时，key 是元素
        '''
        print type(key)
        print type(grp)
        print key
        print grp

    for key, grp in df.groupby(['color','count']):
        '''
        当 groupby 按多列时，key 是元组
        '''
        print type(key)
        print type(grp)
        print key
        print grp

    # groupby 返回 pandas.core.groupby.DataFrameGroupBy
    gb = df.groupby(['color', 'count'])
    print type(gb)

def diff():
    df = pd.DataFrame({'a': [1, 2, 3, 4, 5, 6], 'b': [1, 1, 2, 3, 5, 8], 'c': [1, 4, 9, 16, 25, 36]})
    print df
    # 默认计算同一列，每一行和上一行的差，因而第0行是NaN
    print df.diff()
    # 
    print df.diff(axis=1)
    # periods是减去当前行往前periods的行，不循环
    print df.diff(periods=-1)
    #
    print df.diff(periods=2)

def cumsum():
    df = pd.DataFrame({'a': [1, 2, 3, 4, 5, 6], 'b': [1, 1, 2, 3, 5, 8], 'c': [1, 4, 9, 16, 25, 36]})
    print df
    print df.cumsum()

def read_from_file():
    fn = "text_lost.txt"
    # 如果没设置 names 和 header，则 header=0，以第0行作为标题
    df = pd.read_csv(fn, sep=" ")
    print "pd.read_csv(fn, sep=" ")**************************************"
    print df
    # 使用第2行（从0开始）作为数据的标题，且是数据的起始行
    df = pd.read_csv(fn, sep=" ", header=2)
    print "pd.read_csv(fn, sep=" ", header=2)***************************"
    print df
    # 使用第0行和第2行作为标题，并且会跳过中间的数据
    df = pd.read_csv(fn, sep=" ", header=[0,2])
    print "pd.read_csv(fn, sep=" ", header=[0,2])**********************"
    print df
    # 使用第0列作为行名
    df = pd.read_csv(fn, sep=" ", index_col=0)
    print "pd.read_csv(fn, sep=" ", index_col=0)***********************"
    print df

    # 读入指定列
    df = pd.read_csv(fn, sep=" ", index_col=0, usecols=["name", "passwd"])
    print df

    # 要读入的列名可以是 callable 的
    df = pd.read_csv(fn, sep=" ", usecols = lambda x : x in ["name", "passwd"])
    print df

    # squeeze=True，如果读入一列，则返回 Series
    se = pd.read_csv(fn, sep=" ", squeeze=True, usecols=["name"])
    print type(se)
    df = pd.read_csv(fn, sep=" ",usecols=["name"])
    print type(df)

    df = pd.read_csv(fn, sep=" ", prefix="preX", header=2)
    print df

def apply():
    '''
    测试 apply 函数
    '''
    df = pd.DataFrame({'a': [1, 2, 3, 4, 5, 6], 'b': [1, 1, 2, 3, 5, 8], 'c': [1, 4, 9, 16, 25, 36]})
    print '原始表'
    print df

    print 'sum每列'
    print df.apply(lambda x : x.sum())
    
    print 'sum 每行'
    print df.apply(lambda x : x.sum(), axis=1)

    print '在 apply 中对一列赋值'
    def fun(x):
        x['a'] = x['c'] 
        return x
    print df.apply(fun, axis=1)

    print '在 apply 中对 dataframe 添加新列'
    def fun1(x):
        x['d'] = x['c']
        return x
    print df.apply(fun1, axis=1)

#apply()
#cumsum()
groupby()
#constructDataFrame()
#dml_DataFrame()
#write_to_file(df)
#print df2array(df)
#join()
#diff()
#read_from_file()
