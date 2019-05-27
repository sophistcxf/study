#! -*- coding=utf-8 -*-

import sys
import numpy as np
from scipy import stats

def entropy(l):
    return -sum(l * np.log(l))

def test1():
    l = [1.0/8, 1.0/8, 2.0/8, 3.0/8, 1.0/8]
    # 计算数据分布的熵，注意 entropy 传入的参数是概率密度分布
    print stats.entropy(l)
    print entropy(l)
    pass

if __name__ == '__main__':
    test1()
