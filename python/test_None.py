#! -*- coding=utf-8 -*-

def return_none():
    '''
    如果没有返回值，则返回 None
    '''
    pass

def fun(p=None):
    '''
    None 常用作默认参数
    '''
    print p

if __name__ == "__main__":

    # 注意 n 并且字符串，而是 NoneType
    n = None
    print type(n)
    print n

    # None 作布尔判断时是 False
    if not n:
        print "n is None"

    fun()

    rlt = return_none()
    print type(rlt)


