#-*- coding=utf-8 -*-
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

def test_1():
    ts = pd.Series(np.random.randn(1000), pd.date_range('1/1/2000', periods=1000))
    ts.cumsum()
    ts.plot()
    plt.show()

def test_2():
    ts = pd.Series(np.random.randn(1000), index=pd.date_range('1/1/2000', periods=1000))
    ts.cumsum()
    plt.figure(); ts.plot(style='k--', label='Series'); plt.legend()
    plt.show()

def test_plot_DataFrame():
    ts = pd.Series(np.random.randn(1000), index=pd.date_range('1/1/2000', periods=1000))
    ts.cumsum()
    df = pd.DataFrame(np.random.randn(1000, 4), index=ts.index, columns=list('ABCD'))
    df = df.cumsum()
    plt.figure()
    plot_mode = 2
    if plot_mode == 1:
        df.plot(), plt.legend(loc='best')
    elif plot_mode == 2:
        # 使每一列在单独的subplot中绘制
        df.plot(subplots=True, figsize=(6, 6)); plt.legend(loc='best')

    plt.show()

def test_plot_bar():
    df = pd.DataFrame(np.random.randn(1000, 4), columns=list('ABCD'))
    plt.figure()
    # 取出index那一行，画直方图
    index = 6
    df.iloc[index].plot(kind='bar')
    print df.iloc[index]
    plt.axhline(0, color='k')
    plt.show()

def test_plot_df_bar():
    # 每行是一个族，每列是簇中的一条柱
    df = pd.DataFrame(np.random.rand(10, 4), columns=['a', 'b', 'c', 'd'])
    mode = 3
    if mode == 1:
        df.plot(kind='bar')
    elif mode == 2:
        df.plot(kind='bar', stacked=True)
    elif mode == 3:
        df.plot(kind='barh', stacked=True)
    plt.show()

def test_hist():
    df = pd.DataFrame(np.random.rand(30, 4), columns=['a', 'b', 'c', 'd'])
    df['a'].diff().hist()
    plt.show()

def test_df_box_plot():
    df = pd.DataFrame(np.random.rand(10,5))
    df.boxplot()
    plt.show()

def test_scatter():
    df = pd.DataFrame(np.random.randn(1000, 4), columns=['a', 'b', 'c', 'd'])
    pd.scatter_matrix(df, alpha=0.2, figsize=(6, 6), diagonal='kde')
    plt.show()

#test_2()
#test_plot_bar()
#test_plot_DataFrame()
#test_plot_df_bar()
#test_hist()
#test_df_box_plot()
test_scatter()
