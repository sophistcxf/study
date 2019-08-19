# coding=utf-8

'''
测试 scipy.stats.gamma
'''

from scipy.stats import gamma
import matplotlib.pyplot as plt
import numpy as np

def test_cdf():
    pass

def test_ppf():
    n = 10000
    p = 0.4
    # 累积概率小于 0.01 时的 X
    print '(10000, 0.4) 0.01 ppf', gamma.ppf(0.01, n, p)
    # 累积概率 小于 0.99 时的 X
    print '(10000, 0.4) 0.99 ppf', gamma.ppf(0.99, n, p)

def test_cdf():
    n = 10000
    p = 0.4
    print '(10000, 0.4) 1000 cdf', gamma.cdf(1000, n, p)
    print '(10000, 0.4) 3000 cdf', gamma.cdf(3000, n, p)
    print '(10000, 0.4) 3000 cdf', gamma.cdf(4000, n, p)
    print '(10000, 0.4) 3000 cdf', gamma.cdf(4500, n, p)

def test_pdf():
    n = 10000
    p = 0.4
    print '(10000, 0.4) 中成功 4000 次的概率 ',gamma.pdf(4000, p)
    print '(10000, 0.4) 中成功 3000 次的概率 ',gamma.pdf(3000, p)

def plot_pdf():
    alpha = 4
    beta = 1
    fig, ax = plt.subplots(1, 1)
    x = np.arange(gamma.ppf(0.01, alpha, beta), gamma.ppf(0.99, alpha, beta))
    ax.plot(x, gamma.pdf(x, alpha, beta), 'bo', ms=8, label='gamma pdf X~GE(0.4)')
    plt.show()

#test_pdf()
#test_ppf()
#test_cdf()
plot_pdf()
