# coding=utf-8

'''
模型中的 dual_coef_ 属性是什么？
'''

from sklearn import svm
import matplotlib.pyplot as plt
import numpy as np

def test():
    '''
    数据是线性不可分的
    '''
    X = np.array([[0, 0], [-2,-2],[2,2], [1, 1], [3,3], [0,2], [2,0]])
    y = np.array([0, 0, 1, 1, 1, 0, 0])


    fig, ax = plt.subplots(2,2)
    ax = ax.flatten()

    ax[0].scatter(X[y==0][:,0], X[y==0][:, 1], color="red")
    ax[0].scatter(X[y==1][:,0], X[y==1][:, 1], color="green")

    for i, kernel in enumerate(["linear", "rbf", "poly"]):
        sub_ax = ax[i+1]
        sub_ax.scatter(X[y==0][:,0], X[y==0][:, 1], color="red")
        sub_ax.scatter(X[y==1][:,0], X[y==1][:, 1], color="green")
        clf = svm.SVC(kernel=kernel)
        clf.fit(X, y)
        print "support vectors:"
        print clf.support_vectors_
        print "indices of support vectors: ", clf.support_
        print "number of support vectors for each class: ", clf.n_support_
        if kernel == "linear":
            print "coef_: ", clf.coef_
        print "dual_coef_: ", clf.dual_coef_
        print "score: ", clf.score(X, y)

        print np.dot(clf.dual_coef_, clf.support_vectors_)


test()
