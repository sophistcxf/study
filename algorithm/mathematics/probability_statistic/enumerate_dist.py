# coding=utf-8
'''
罗列各种分布
'''

import scipy.stats as ss
import numpy.random as nr

def binom():
    n = 100
    p = 0.4
    # 均值、方差、偏度、峰度
    meam, var, skew, kurt = ss.binom.stats(n, p, moments='mvsk')
    help(ss.binom)
    print len(nr.binomial(n, p, 10000))

def geo():
    '''
    几何分布
    '''
    help(ss)


#binom()
geo()
