#! -*- coding=utf-8 -*-

"""
测试 color 参数
matplotlib 中许多函数都有 color 参数，本示例测试 color 参数的用法
[1] https://matplotlib.org/api/_as_gen/matplotlib.pyplot.colors.html
[2] https://htmlcolorcodes.com/
"""

import numpy as np
import matplotlib.pyplot as plt

print __doc__

def color_test():
    x = np.arange(0, 20, 0.1)
    y = np.sin(x)
    plt.plot(x, y, color='r')
    # 使用 html color codes
    plt.plot(x, y+1, color='#0b3494')
    # 使用 tuple
    plt.plot(x, y+2, color=(0.0, 1.0, 1.0))
    plt.show()

color_test()
