#! -*- coding=utf-8 -*-

'''
根据已知 array，生成同样 shape 的 array
'''

print __doc__

import numpy as np

arr = np.array([[1,2,3], [4,5,6]])
print arr
print np.zeros_like(arr)
print np.empty_like(arr)
print np.ones_like(arr)
print np.full_like(arr, 2)