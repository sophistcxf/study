import numpy as np
import similaritymeasures
import matplotlib.pyplot as plt


def test1():
    x1 = [ i for i in range(0, 100, 1)]
    y1 = [ i for i in range(0, 100, 1)]
    x2 = [ i for i in range(0, 50, 1)]
    y2 = [ i + 2 for i in x2 ]  
    plt.plot(x1, y1, x2, y2)
    plt.show()
    curve1 = [x1, y1]
    curve2 = [x2, y2]
    df = similaritymeasures.frechet_dist(curve1, curve2)
    print df

test1()
