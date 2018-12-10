import numpy as np

def mask_array():
    arr = np.arange(1, 10, 1)
    b = arr % 2 == 0
    print arr
    print b
    print arr[b]


mask_array()
