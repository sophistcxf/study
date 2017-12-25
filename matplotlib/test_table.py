#!-*- coding=utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt

def test1():
    fig, axs =plt.subplots(2,1)
    clust_data = np.random.random((10,3))
    collabel=("col 1", "col 2", "col 3")
    axs[0].axis('tight')
    axs[0].axis('off')
    axs[0].xaxis.set_visible(False)
    axs[0].yaxis.set_visible(False)
    the_table = axs[0].table(cellText=clust_data,colLabels=collabel,loc='center')

    axs[1].plot(clust_data[:,0],clust_data[:,1])
    plt.show()

def test2():
    data = [[ 66386, 174296,  75131, 577908,  32015],
            [ 58230, 381139,  78045,  99308, 160454],
            [ 89135,  80552, 152558, 497981, 603535],
            [ 78415,  81858, 150656, 193263,  69638],
            [139361, 331509, 343164, 781380,  52269]]

    columns = ('Freeze', 'Wind', 'Flood', 'Quake', 'Hail')
    rows = ['%d year' % x for x in (100, 50, 20, 10, 5)]

    values = np.arange(0, 2500, 500)
    value_increment = 1000

    # Get some pastel shades for the colors
    # 得到5行，每行是一个四维向量，是rgba吗？
    colors = plt.cm.BuPu(np.linspace(0, 0.5, len(rows)))
    n_rows = len(data)

    index = np.arange(len(columns)) + 0.3
    bar_width = 0.4

    # Initialize the vertical-offset for the stacked bar chart.
    y_offset = np.zeros(len(columns))

    # Plot bars and create text labels for the table
    cell_text = []
    for row in range(n_rows):
        plt.bar(index, data[row], bar_width, bottom=y_offset, color=colors[row])
        y_offset = y_offset + data[row]
        cell_text.append(['%1.1f' % (x / 1000.0) for x in y_offset])
    # Reverse colors and text labels to display the last value at the top.
    # 还有这种写法
    colors = colors[::-1]
    cell_text.reverse()

    # Add a table at the bottom of the axes
    # cell_text是一个二维数组，rowLabels是行名，colLabels是行名，rowColours是行的颜色
    the_table = plt.table(cellText=cell_text,
                          rowLabels=rows,
                          rowColours=colors,
                          colLabels=columns,
                          loc='bottom')

    # Adjust layout to make room for the table:
    plt.subplots_adjust(left=0.2, bottom=0.2)

    plt.ylabel("Loss in ${0}'s".format(value_increment))
    #设置了y轴的tick的值和label的对应关系，就是说label显示500（由values中取的值），但实际对应的是500000
    plt.yticks(values * value_increment, ['%d' % val for val in values])
    #设置x轴的tick是空，效果上是使用了table的rowLabels
    plt.xticks([])
    plt.title('Loss by Disaster')

    plt.show()

def test3():
    cell_text = []
    for i in range(10):
        cell_text.append(np.linspace(0,i,10))

    the_table = plt.table(cellText=cell_text,
                          rowLabels=["%s row" % i for i in range(10)],
                          colLabels=["%s col" % i for i in range(10)])
    plt.show()

test1()
