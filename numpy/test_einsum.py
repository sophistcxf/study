# coding=utf-8

import numpy as np

a = np.array([1, 2, 3])
b = np.array([4, 5, 6])

print np.einsum('i', a)

# i重复，所以是 sum(a(i)*b(i))，等价于内积
print np.einsum('i,i', a, b)
print np.inner(a, b)

# 取对角线的和
a = np.array([[1,2,3], [4,5,6], [7,8,9]])
print np.einsum('ii', a)
print np.trace(a)
#print np.einsum('ij,jk', a, b)
