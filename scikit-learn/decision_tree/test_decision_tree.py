import numpy as np
import matplotlib.pyplot as plt
 
from sklearn.datasets import load_iris
from sklearn.tree import DecisionTreeClassifier
import sklearn.tree as tree
import graphviz

def tree_to_code(tree, feature_names):
    tree_ = tree.tree_
    feature_name = [
        feature_names[i] if i != _tree.TREE_UNDEFINED else "undefined!"
        for i in tree_.feature
    ]
    print "def tree({}):".format(", ".join(feature_names))

    def recurse(node, depth):
        indent = "  " * depth
        if tree_.feature[node] != _tree.TREE_UNDEFINED:
            name = feature_name[node]
            threshold = tree_.threshold[node]
            print "{}if {} <= {}:".format(indent, name, threshold)
            recurse(tree_.children_left[node], depth + 1)
            print "{}else:  # if {} > {}".format(indent, name, threshold)
            recurse(tree_.children_right[node], depth + 1)
        else:
            print "{}return {}".format(indent, tree_.value[node].tolist())

    recurse(0, 1) 

 
# Load data
iris = load_iris()
 
for pairidx, pair in enumerate([[0, 1], [0, 2], [0, 3],
                                [1, 2], [1, 3], [2, 3]]):
    # We only take the two corresponding features
    X = iris.data[:, pair]
    y = iris.target
 
    # Train
    clf = DecisionTreeClassifier().fit(X, y)
    print "score: %f" % clf.score(X, y)
 
    dot_data = tree.export_graphviz(clf, out_file=None)
    graph = graphviz.Source(dot_data)
    graph.render("dd.dot")

    #tree_to_code(clf, ["fea1", "fea2"])

    exit(0)
