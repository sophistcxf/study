#! -*- coding=utf-8 -*-
import numpy as np

# 沿 axis=0 拼接
print np.r_[[[1,2],[3,4]],[[5,6],[7,8]]]
print np.r_[[1,2,3],[4,5,6]]
print np.r_['r',[1,2,3], [4,5,6]]

