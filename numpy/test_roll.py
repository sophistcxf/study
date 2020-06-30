# coding=utf-8

import numpy as np

arr = np.array([1, 2, 3, 4, 5, 6, 7, 8])

print arr
print np.roll(arr, 1)
print np.roll(arr, 3)
print np.roll(arr, -2)

print '二维数组'
arr = np.array([[1, 2, 3 ,4], [5, 6, 7, 8,], [9, 10, 11, 12]])
print arr
print 'np.roll(arr, 3):'
print np.roll(arr, 3)
print 'np.roll(arr, 3, 0):'
print np.roll(arr, 2, 0)
print 'np.roll(arr, 3, 1):'
print np.roll(arr, 3, 1)
#print np.roll(arr, 2, 3) #error
