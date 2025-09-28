#! -*- coding=utf-8 -*-

def test1():
    '''
    伯努力分布的方差
    '''
    x = [1, 0, 0, 1, 1, 1, 0, 1]
    p = len([ i for i in x if i == 1]) * 1. / len(x)
    print "var = %f" % (p*(1-p))

    mean = sum(x) * 1. / len(x)
    var = sum([pow((i - mean),2) for i in x]) / len(x)
    print "var = %f" % var

if __name__ == "__main__":
    test1()
