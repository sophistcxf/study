# coding=utf-8

'''
测试 scipy.stats.geom
'''

from scipy.stats import geom
import matplotlib.pyplot as plt
import numpy as np

def test_cdf():
    pass

def test_ppf():
    n = 10000
    p = 0.4
    # 累积概率小于 0.01 时的 X
    print '(10000, 0.4) 0.01 ppf', geom.ppf(0.01, n, p)
    # 累积概率 小于 0.99 时的 X
    print '(10000, 0.4) 0.99 ppf', geom.ppf(0.99, n, p)

def test_cdf():
    n = 10000
    p = 0.4
    print '(10000, 0.4) 1000 cdf', geom.cdf(1000, n, p)
    print '(10000, 0.4) 3000 cdf', geom.cdf(3000, n, p)
    print '(10000, 0.4) 3000 cdf', geom.cdf(4000, n, p)
    print '(10000, 0.4) 3000 cdf', geom.cdf(4500, n, p)

def test_pmf():
    n = 10000
    p = 0.4
    print '(10000, 0.4) 中成功 4000 次的概率 ',geom.pmf(4000, p)
    print '(10000, 0.4) 中成功 3000 次的概率 ',geom.pmf(3000, p)

def plot_pmf():
    p = 0.4
    fig, ax = plt.subplots(1, 1)
    x = np.arange(geom.ppf(0.01, p), geom.ppf(0.99, p))
    ax.plot(x, geom.pmf(x, p), 'bo', ms=8, label='geom pmf X~GE(0.4)')
    plt.show()

#test_pmf()
#test_ppf()
#test_cdf()
plot_pmf()
