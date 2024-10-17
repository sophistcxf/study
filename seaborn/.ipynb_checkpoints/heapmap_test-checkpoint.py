import numpy as np; np.random.seed(0)
import seaborn as sns; sns.set()
import matplotlib.pyplot as plt
import pandas as pd

def test_simple():
    uniform_data = np.random.rand(10, 12)
    ax = sns.heatmap(uniform_data)
    plt.show()

def test_dataframe():
    df = pd.DataFrame({'a': [1, 2, 3, 4, 5, 6], 'b': [1, 1, 2, 3, 5, 8], 'c': [1, 4, 9, 16, 25, 36],
                       'd': [11,22,33,44,55,66]}, index=['r1', 'r2', 'r3', 'r4', 'r5', 'r6'])
    sns.heatmap(df)
    plt.show()

#test_simple()
test_dataframe()
