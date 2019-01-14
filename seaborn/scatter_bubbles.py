"""
Scatterplot with varying point sizes and hues
==============================================

_thumb: .45, .5

"""
import seaborn as sns
import matplotlib.pyplot as plt
sns.set(style="white")

# Load the example mpg dataset, mpg is pandas.DataFrame
mpg = sns.load_dataset("mpg")
print mpg.columns
print mpg.head(10)

# Plot miles per gallon against horsepower with other semantics
sns.relplot(x="horsepower", y="mpg", hue="origin", size="weight",
            sizes=(40, 400), alpha=.5, palette="muted",
            height=6, data=mpg)

#plt.show()
