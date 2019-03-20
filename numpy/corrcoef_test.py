import numpy as np
import copy

arr1 = np.random.normal(0, 2.0, size=10)
arr1.sort()
arr2 = np.random.poisson(size=10)
arr2.sort()
arr3 = arr1 + 10
arr3.sort()
arr4 = np.random.normal(0, 2.0, size=10)
arr4.sort()
arr5 = copy.deepcopy(arr1)
arr5.sort()

print np.corrcoef([arr1, arr2, arr3, arr4, arr5])