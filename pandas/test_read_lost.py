#-*- coding=utf-8 -*-

import pandas as pd
import numpy as np
import math

'''
从含有缺失值的文件中读取数据
'''

fn = "text_lost.txt"

df = pd.read_csv(fn, sep=" ")

print df
print df.shape

# 可以看出，缺失数据不影响数据类型
print df.dtypes

# 按条件从df中取行时，一定要加小括号
print df[(np.isnan(df.age)) | (df.age > 20)]

# 不能使用math.isnan
try:
    print df[(math.isnan(df.age)) | (df.age > 20)]
except:
    print "cat not use math.isnan"
