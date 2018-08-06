from sklearn import svm

def test1():
    X = [[0, 0], [-2,-2],[2,2], [1, 1], [3,3]]
    y = [0, 0, 1, 1, 1]
    clf = svm.SVC()
    clf.fit(X, y)
    print "support vectors:"
    print clf.support_vectors_
    print "indices of support vectors:"
    print clf.support_
    print "number of support vectors for each class:"
    print clf.n_support_
    print "predict result:"
    print clf.predict([[2., 2.], [-1., -1.]])

test1()
