import seaborn as sns
import matplotlib.pyplot as plot
import numpy as np
sns.set(style="ticks")

# Load the example dataset for Anscombe's quartet
df = sns.load_dataset("anscombe")
df = df[np.logical_or(df["dataset"]=="I", df["dataset"]=="II")]
#df = df[(df["dataset"]=="I") | (df["dataset"]=="II")]
print df

# Show the results of a linear regression within each dataset
facets = sns.lmplot(x="x", y="y", col="dataset", hue="dataset", data=df,
           col_wrap=2, ci=None, palette="muted", size=3,
           scatter_kws={"s": 50, "alpha": 1}, legend=True, legend_out=True)
#print type(facets) # <class 'seaborn.axisgrid.FacetGrid'>
#plot.show()
