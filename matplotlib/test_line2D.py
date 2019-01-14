#! -*- coding=utf-8 -*-

import matplotlib.pyplot as plt
import numpy as np

def test():
    t = np.arange(0.0, 2.0, 0.01)
    s = 1 + np.sin(2 * np.pi * t)

    l = plt.plot(t, s)
    line2d = l[0]

    # 得到 line2d 的颜色
    color = line2d.get_color()
    print color

    # 提到 line2d 的数据
    xdata, ydata = line2d.get_data()
    print type(xdata),type(ydata)

    style = line2d.get_linestyle()
    print style

    width = line2d.get_linewidth()
    print width

    # 设置透明度
    line2d.set_alpha(220)

    plt.show()

if __name__ == "__main__":
    test()
