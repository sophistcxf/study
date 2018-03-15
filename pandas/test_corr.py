import numpy as np
import pandas as pd

df = pd.DataFrame(np.random.randint(low=0, high=10, size=(3,5)),
                columns=['a','b','c','d','e'])

print df.iloc[:,1]
print df.corr()
