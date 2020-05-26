# coding=utf-8

from sklearn import svm
import matplotlib.pyplot as plt
import numpy as np

def make_meshgrid(x, y, h=.02):
    x_min, x_max = x.min() - 1, x.max() + 1
    y_min, y_max = y.min() - 1, y.max() + 1
    xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                         np.arange(y_min, y_max, h))
    return xx, yy

def test1():
    '''
    数据本身是线性可分的
    '''
    X = np.array([[0, 0], [-2,-2],[2,2], [1, 1], [3,3]])
    y = np.array([0, 0, 1, 1, 1])

    plt.scatter(X[y==0][:,0], X[y==0][:, 1], color="red")
    plt.scatter(X[y==1][:,0], X[y==1][:, 1], color="green")

    clf = svm.SVC(kernel="linear")
    clf.fit(X, y)
    print "support vectors:"
    print clf.support_vectors_
    print "indices of support vectors:"
    print clf.support_
    print "number of support vectors for each class:"
    print clf.n_support_
    print "coef_: ", clf.coef_
    print "dual_coef_: ", clf.dual_coef_
    print "score: ", clf.score(X, y)
    print clf.predict([[2., 2.], [-1., -1.]])
    xx, yy = make_meshgrid(X[:,0], X[:,1])

    Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])
    Z = Z.reshape(xx.shape)
    plt.contourf(xx, yy, Z, alpha=0.5)

    plt.show()

def test2():
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
        print clf.predict([[2., 2.], [-1., -1.]])

        xx, yy = make_meshgrid(X[:,0], X[:,1])

        Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])
        Z = Z.reshape(xx.shape)
        sub_ax.contourf(xx, yy, Z, alpha=0.5)

    '''
    以上例子，模型的C=1，rbf和poly未完全拟合，得分是0.87
    通过修改C，增大分类错误的惩罚，能更好的拟合训练样本
    '''
    fig, ax = plt.subplots(2,2)
    ax = ax.flatten()

    ax[0].scatter(X[y==0][:,0], X[y==0][:, 1], color="red")
    ax[0].scatter(X[y==1][:,0], X[y==1][:, 1], color="green")
    for i, kernel in enumerate(["linear", "rbf", "poly"]):
        sub_ax = ax[i+1]
        sub_ax.scatter(X[y==0][:,0], X[y==0][:, 1], color="red")
        sub_ax.scatter(X[y==1][:,0], X[y==1][:, 1], color="green")
        clf = svm.SVC(kernel=kernel, C=100)
        clf.fit(X, y)
        print "support vectors:"
        print clf.support_vectors_
        print "indices of support vectors: ", clf.support_
        print "number of support vectors for each class: ", clf.n_support_
        if kernel == "linear":
            print "coef_: ", clf.coef_
        print "dual_coef_: ", clf.dual_coef_
        print "score: ", clf.score(X, y)
        print clf.predict([[2., 2.], [-1., -1.]])

        xx, yy = make_meshgrid(X[:,0], X[:,1])

        Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])
        Z = Z.reshape(xx.shape)
        sub_ax.contourf(xx, yy, Z, alpha=0.5)


    plt.show()

test2()
