import pandas as pd
import numpy as np

def test_covariance():
    s1 = pd.Series(np.random.randn(1000))
    s2 = pd.Series(np.random.randn(1000))
    print s1.cov(s2)

def test_correlation():
    frame = pd.DataFrame(np.random.randn(1000, 5), columns=['a', 'b', 'c', 'd', 'e'])
    frame.iloc[::2] = np.nan
    print frame['a'].corr(frame['b'])

#test_covariance()
test_correlation()
