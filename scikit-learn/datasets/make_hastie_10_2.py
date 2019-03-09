# -*- coding=utf-8 -*-

from sklearn import datasets


"""
Hastie et al. 2009, Example 10.2. 中所用的数据集
X 是 10 个特征，standard independent Gaussian，是说每一个特征都符合标准正态分布吗？
y 的计算是：y[i] = 1 if np.sum(X[i] ** 2) > 9.34 else -1
此数据集用于二分类
"""

def plot_features_dist():
    X, y = datasets.make_hastie_10_2(n_samples=12000, random_state=1)
    