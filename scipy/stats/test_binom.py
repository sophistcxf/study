# coding=utf-8

'''
测试 scipy.stats.binom
'''

from scipy.stats import binom
import matplotlib.pyplot as plt
import numpy as np

def test_cdf():
    pass

def test_ppf():
    n = 10000
    p = 0.4
    # 累积概率小于 0.01 时的 X
    print '(10000, 0.4) 0.01 ppf', binom.ppf(0.01, n, p)
    # 累积概率 小于 0.99 时的 X
    print '(10000, 0.4) 0.99 ppf', binom.ppf(0.99, n, p)

def test_cdf():
    n = 10000
    p = 0.4
    print '(10000, 0.4) 1000 cdf', binom.cdf(1000, n, p)
    print '(10000, 0.4) 3000 cdf', binom.cdf(3000, n, p)
    print '(10000, 0.4) 3000 cdf', binom.cdf(4000, n, p)
    print '(10000, 0.4) 3000 cdf', binom.cdf(4500, n, p)

def test_pmf():
    n = 10000
    p = 0.4
    print '(10000, 0.4) 中成功 4000 次的概率 ',binom.pmf(4000, n, p)
    print '(10000, 0.4) 中成功 3000 次的概率 ',binom.pmf(3000, n, p)

def plot_pmf():
    n = 10000
    p = 0.4
    fig, ax = plt.subplots(1, 1)
    x = np.arange(binom.ppf(0.01, n, p), binom.ppf(0.99, n, p))
    ax.plot(x, binom.pmf(x, n, p), 'bo', ms=8, label='binom pmf (10000, 0.4)')
    plt.show()

#test_pmf()
#test_ppf()
#test_cdf()
plot_pmf()
