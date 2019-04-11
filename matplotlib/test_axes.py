import matplotlib.pyplot as plt
import numpy as np

def figure_without_axes():
    fig = plt.figure()  # an empty figure with no axes
    fig.suptitle('with no axes')
    plt.show()

def figure_with_axes():
    fig, ax_list = plt.subplots(2, 2) # create a figure with 2*2 axes
    print ax_list
    fig.suptitle('with 4 axes')
    x = np.arange(0, 10, 0.2)
    y = np.sin(x)
    l = ax_list[0][0].plot(x, y)
    ax_list[0][0].set_title('sin')
    print l
    plt.show()

figure_with_axes()
