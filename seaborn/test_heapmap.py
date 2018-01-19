import numpy as np; np.random.seed(0)
import seaborn as sns; sns.set()
import matplotlib.pyplot as plt

def test_simple():
    uniform_data = np.random.rand(10, 12)
    ax = sns.heatmap(uniform_data)
    plt.show()

test_simple()
