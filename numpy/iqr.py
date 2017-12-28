'''
compute iqr using numpy
'''

import numpy as np

def iqr(data):
    #np.subtract(*np.percentile(data, [75, 25]))
    p25, p75 = np.percentile(data, [25, 75], interpolation='nearest')
    i = p75 - p25
    return i, p25, p75

d=[35.0,35.0,550.0,895.0,126.0,37.0,525.0,10.0,33.0,31.0,10.0,352.0,196.0,318.0,336.0,300.0,266.0,20.0,803.0,650.0,15.0,466.0,9.0,40.0,985.0,225.0]

print iqr(d)
