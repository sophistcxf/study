from sklearn import svm

def test1():
    X = [[0, 0], [2, 2]]
    y = [0.5, 2.5]
    clf = svm.SVR()
    clf.fit(X, y)
    print clf.predict([[1, 1]])

test1()
