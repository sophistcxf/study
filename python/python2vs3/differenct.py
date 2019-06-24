#! coding=utf-8

"""
测试 python2 与 python3 的区别
参考：
[1] https://sebastianraschka.com/Articles/2014_python_2_3_key_diff.html
"""
import sys

print(sys.version_info.major)

def test():
    a = 10
    b = 20
    '''
    python2 打印 (10,20) 是个元组
    python3 打印 10 20
    '''
    print(a, b)

'''
def only_python2():
    print 'hello world'
'''

def int_division():
    '''
    这个比较容易出错
    '''
    print('3 / 2 =', 3 / 2)
    print('3 // 2 =', 3 // 2)
    print('3 / 2.0 =', 3 / 2.0)
    print('3 // 2.0 =', 3 // 2.0)

#test()
int_division()
