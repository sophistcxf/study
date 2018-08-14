#!-*- coding=utf-8 -*-
"""
=========================================
SVM: Maximum margin separating hyperplane
=========================================

Plot the maximum margin separating hyperplane within a two-class
separable dataset using a Support Vector Machine classifier with
linear kernel.
"""
print(__doc__)

import numpy as np
import matplotlib.pyplot as plt
from sklearn import svm
from sklearn.datasets import make_blobs


# we create 40 separable points
X, y = make_blobs(n_samples=40, centers=2, random_state=6)

# fit the model, don't regularize for illustration purposes
clf = svm.SVC(kernel='linear', C=1000)
clf.fit(X, y)

plt.scatter(X[:, 0], X[:, 1], c=y, s=30, cmap=plt.cm.Paired)

# plot the decision function
ax = plt.gca()
xlim = ax.get_xlim()
ylim = ax.get_ylim()

def the_shape(n, v):
    print n,": ",v.shape

# create grid to evaluate model
xx = np.linspace(xlim[0], xlim[1], 30)
the_shape("xx", xx)
yy = np.linspace(ylim[0], ylim[1], 30)
the_shape("yy", yy)
YY, XX = np.meshgrid(yy, xx)
the_shape("XX", XX)
the_shape("YY", YY)
xy = np.vstack([XX.ravel(), YY.ravel()]).T
the_shape("xy", xy)
# 使用 decision_function 对 30*30 网格上的值进行预测
Z = clf.decision_function(xy).reshape(XX.shape)
the_shape("Z", Z)

# plot decision boundary and margins
# 画等高线，只画值为-1,0,1的三条等高线，0 即为分割平面
ax.contour(XX, YY, Z, colors=['b','r','b'], levels=[-1, 0, 1], alpha=0.5,
           linestyles=['--', '-', '--'])
# plot support vectors
ax.scatter(clf.support_vectors_[:, 0], clf.support_vectors_[:, 1], s=100,
           linewidth=1, facecolors='none')
plt.show()
