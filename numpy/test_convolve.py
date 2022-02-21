import numpy as np

print np.convolve([1, 2, 3], [0, 1, 0.5])
print np.convolve([1, 2, 3], [0, 1, 0.5], 'same')
print np.convolve([1, 2, 3], [0, 1, 0.5], 'valid')
