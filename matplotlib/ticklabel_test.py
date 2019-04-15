#! -*- coding=utf-8 -*-
import matplotlib.pyplot as plt

def label_in_plot():
    """
    plot 中的 xticks
    """
    x = ['China', 'American', 'England', 'France', 'German', 'Russian']
    y = [1, 2, 3, 4, 5, 1]
    plt.plot(x, y)
    # 获取当前的 Axes
    ax = plt.gca()
    print type(ax)
    for label in ax.get_xticklabels():
        print type(label)
        # xticklabel 顺时针旋转 270 度
        label.set_rotation(270)
    plt.show()

if __name__ == "__main__":
    label_in_plot()
