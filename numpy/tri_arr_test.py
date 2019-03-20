#! -*- coding=utf-8 -*-

'''
根据已知 array，生成同样 shape 的 array
'''

print __doc__

import numpy as np

arr = np.random.rand(5, 5)
print arr

# triu_indices_from 和 tril_indices_from 返回的是索引，而不是索引上的元素，如果需要元素，需要再取一次
# upper-triangle
print arr[np.triu_indices_from(arr)]
# lower-triangle
print arr[np.tril_indices_from(arr)]