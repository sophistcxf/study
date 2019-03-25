import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

from sklearn import ensemble
from sklearn import datasets
from sklearn.metrics import accuracy_score
from sklearn import tree
import graphviz

def analyze_clf(clf):
    for idx, i in enumerate(clf.estimators_):
        sub_estimator = i[0]
        help(sklearn.tree)
        #dot_str = tree.export_graphviz(sub_estimator)
        #print dot_str
        #s = graphviz.Source(dot_str)
        #s.render(filename='tree_image/%d' % idx, format='pdf')

def test():
    X, y = datasets.make_hastie_10_2(n_samples=12000, random_state=1)
    X = X.astype(np.float32)

    labels, y = np.unique(y, return_inverse=True)

    X_train, X_test = X[:2000], X[2000:]
    y_train, y_test = y[:2000], y[2000:]

    df = pd.DataFrame(X_train)
    print df.describe()
    s = pd.Series(y_train)
    s = s.astype('str')
    print s.describe()

    clf = ensemble.GradientBoostingClassifier(n_estimators=50)
    clf.fit(X_train, y_train)

    analyze_clf(clf)

    print clf.score(X_train, y_train)
    print clf.score(X_test, y_test)

if __name__ == "__main__":
    test()
