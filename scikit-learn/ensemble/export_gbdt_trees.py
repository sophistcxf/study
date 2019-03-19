import numpy as np
import matplotlib.pyplot as plt

from sklearn import ensemble
from sklearn import datasets
from sklearn.utils import shuffle
from sklearn.metrics import mean_squared_error
from sklearn import tree
import pydotplus

boston = datasets.load_boston()
X, y = shuffle(boston.data, boston.target, random_state=13)
X = X.astype(np.float32)
offset = int(X.shape[0] * 0.9)
X_train, y_train = X[:offset], y[:offset]
X_test, y_test = X[offset:], y[offset:]

# #############################################################################
# Fit regression model
params = {'n_estimators': 500, 'max_depth': 4, 'min_samples_split': 2,
          'learning_rate': 0.01, 'loss': 'ls'}
clf = ensemble.GradientBoostingRegressor(**params)

clf.fit(X_train, y_train)
mse = mean_squared_error(y_test, clf.predict(X_test))
print("MSE: %.4f" % mse)
print clf.estimators_.shape
help(tree.export_graphviz)
for i in range(0, 1):
    sub_tree = clf.estimators_[i, 0]
    dot_data = tree.export_graphviz(
        sub_tree, out_file="%d.dot" % i, filled=True,
        rounded=True,
        special_characters=True,
        proportion=True
    )
    #print(dot_data)
    #graph = pydotplus.graph_from_dot_data(dot_data)
    #help(graph.create_png)

    #print dot_str