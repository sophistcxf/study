#! coding:utf-8

import numpy as np
import copy

def data1():
    '''
    x 是身高，单位为米
    y 是体重，单位为千克
    '''
    x = [1, 1.5, 1.4, 1.5, 2, 1.7, 1.77, 1.78, 1.55, 1.65, 1.8]
    y = [30, 40, 41, 38, 200, 65, 70, 75, 50, 55, 80]
    return (x,y)

def test1():
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


def test2():
    '''
    由此例可以看到，协方差随着量纲的变化而变化，但相关系数不随量纲变化而变化
    '''
    (x, y) = data1()
    print np.corrcoef([x ,y])
    print np.cov([x ,y])

    x = [ i*100 for i in x ]
    y = [ i*1000 for i in y ]
    print np.corrcoef([x, y])
    print np.cov([x, y])

test2()
