#!-*- coding=utf-8 -*-
import numpy as np

a = np.array([[1, 2], [3, 4]])
b = np.array([[5, 6]])

# 将[5,6]拼接在axis=0上
print np.concatenate((a,b), axis=0)
# 将[5,6]拼接在axis=1上
print np.concatenate((a,b.T), axis=1)

# 按 axis=0 拼接
print np.r_[a,b]
