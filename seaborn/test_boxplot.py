import seaborn as sns
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def test1():
    sns.set(style="ticks")

    # Load the example tips dataset
    tips = sns.load_dataset("tips")

    # Draw a nested boxplot to show bills by day and sex
    sns.boxplot(x="day", y="total_bill", hue="sex", data=tips, palette="PRGn")
    sns.despine(offset=10, trim=True)
    plt.show()

def test2():
    sns.set(style="ticks")

    a = np.linspace(20, 30, 100)
    b = np.linspace(10, 40, 100)
    c = np.arange(100,110,1)

    df = pd.DataFrame(np.concatenate((a, b, c)), columns=["x"])
    
    sns.boxplot(x="x", data=df, palette="PRGn")
    sns.despine(offset=10, trim=True)

    plt.show()

test2()
