from scipy import stats
import numpy as np

x = np.linspace(-15, 15, 9)

print stats.kstest(x, 'norm')

np.random.seed(987654321)
print stats.kstest('norm', False, N=100)

np.random.seed(987654321)
print stats.kstest(stats.norm.rvs(size=100), 'norm')
