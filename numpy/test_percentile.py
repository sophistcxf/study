import numpy as np
import math
import copy

def percentile(data, l):
    t = copy.deepcopy(data)
    t.sort()
    result = []
    for i in l:
        idx = i * 1. / 100 * (len(t)-1)
        if int(idx) == idx:
            result.append(t[int(idx)])
        else:
            beg = t[int(math.floor(idx))]
            result.append(beg)
    return tuple(result) 

def outlier(data):
    q25, q75 = np.percentile(data, [25, 75], interpolation="lower")
    iqr = q75 - q25
    low_guard = q25 - iqr * 1.5
    high_guard = q75 + iqr * 1.5
    outlier = []
    for i in data:
        if i < low_guard or i > high_guard:
            outlier.append(i)
    return outlier,low_guard,q25,q75,high_guard


def outlier2(data):
    q25, q75 = percentile(data, [25, 75])
    iqr = q75 - q25
    low_guard = q25 - iqr * 1.5
    high_guard = q75 + iqr * 1.5
    outlier = []
    for i in data:
        if i < low_guard or i > high_guard:
            outlier.append(i)
    return outlier,low_guard,q25,q75,high_guard
def test1():
    arr = np.arange(0, 90, 9)
    print arr
    inter_method = ["linear", "lower", "higher", "nearest", "midpoint"]
    for m in inter_method:
        print np.percentile(arr, [25, 75], interpolation=m)


d=[650.0,10.0]
print outlier(d)
print outlier2(d)
