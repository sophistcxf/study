import seaborn as sns
import matplotlib.pyplot as plot
sns.set(style="ticks")

# Load the example dataset for Anscombe's quartet
df = sns.load_dataset("anscombe")

# Show the results of a linear regression within each dataset
'''
sns.lmplot(x="x", y="y", col="dataset", hue="dataset", data=df,
           col_wrap=2, ci=None, palette="muted", size=3,
           scatter_kws={"s": 50, "alpha": 1}, legend=True, legend_out=True)
           '''
sns.lmplot(x="x", y="y", col="dataset", data=df,
           col_wrap=2,ci=None, palette="muted", size=3,
           scatter_kws={"s": 50, "alpha": 1}, legend=True, legend_out=True)
plot.show()
