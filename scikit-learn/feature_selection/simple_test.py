#! -*- coding=utf-8 -*-

'''
测试特征选择的方法
参考
[1] https://scikit-learn.org/stable/modules/feature_selection.html
'''

def test1():
    '''
    过滤掉方差比较小的特征
    '''
    from sklearn.feature_selection import VarianceThreshold

    # 对于二值特征来说，服从伯努力分布，其方差是 p(1-p)
    X = [[0, 0, 1], [0, 1, 0], [1, 0, 0], [0, 1, 1], [0, 1, 0], [0, 1, 1]]
    threshold = .8 * (1. - .8)
    sel = VarianceThreshold(threshold=threshold)
    print sel.fit_transform(X)
    print "threshold: %f" % threshold
    for idx in range(3):
        p = len([ i[idx] for i in X if i[idx] == 0 ]) * 1. / len(X)
        print p * (1 - p)

def test2():
    '''
    '''
    from sklearn.datasets import load_iris
    from sklearn.feature_selection import SelectKBest
    from sklearn.feature_selection import chi2
    iris = load_iris()
    X, y = iris.data, iris.target
    X_new = SelectKBest(chi2, k=2).fit_transform(X, y)

def test3():
    from sklearn.svm import LinearSVC
    from sklearn.datasets import load_iris
    from sklearn.feature_selection import SelectFromModel
    iris = load_iris()
    X, y = iris.data, iris.target
    svc = LinearSVC(C=0.01, penalty="l1", dual=False).fit(X, y)
    model = SelectFromModel(lsvc, prefit=True)
    X_new = model.transform(X)


if __name__ == "__main__":
    test1()
