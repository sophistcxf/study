import numpy as np

def test1():
    arr = np.arange(0, 90, 9)
    print arr
    inter_method = ["linear", "lower", "higher", "nearest", "midpoint"]
    for m in inter_method:
        print np.percentile(arr, [25, 75], interpolation=m)

test1()
