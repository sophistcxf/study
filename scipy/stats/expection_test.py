import scipy.stats as stats
import numpy as np

m = lambda k: (0.12*np.sin((2*np.pi*k)/3 + np.pi/2))**2
k=[1,2,3]
uniform_dist = stats.rv_discrete(values=(k, np.ones(k)/len(k)))
result = uniform_dist.expect(m)
help(uniform_dist.expect)
print result
