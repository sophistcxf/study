#! -*- coding=utf-8 -*-

'''
测试 decision tree 的内部原理
sklearn 使用 CART 树，结合《统计学习方法》，探索 decision tree 的内部实现，
佐证对这一算法的理解
'''

print __doc__

import pandas as pd

def test1():
    '''
    读取 tjxxff_5_1_table.txt 中的数据 
    '''
    df = pd.read_csv('tjxxff_5_1_table.txt', sep=',')
