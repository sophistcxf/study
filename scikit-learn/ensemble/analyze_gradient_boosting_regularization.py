import numpy as np
import matplotlib.pyplot as plt

from sklearn import ensemble
from sklearn import datasets

X, y = datasets.make_hastie_10_2(n_samples=12000, random_state=1)
X = X.astype(np.float32)

labels, y = np.unique(y, return_inverse=True)

X_train, X_test = X[:2000], X[2000:]
