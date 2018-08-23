from sklearn.neural_network import MLPClassifier
import sys

def output_mlp(mlp):
    print "coefs:", [coef for coef in mlp.coefs_]
    print "n_layers_:", mlp.n_layers_
    print "intercepts_:", mlp.intercepts_
    print "loss_:", mlp.loss_
    print "n_outputs_:", mlp.n_outputs_
    print "out_activation_:", mlp.out_activation_
    print "classes_:", mlp.classes_


def test1():
    X = [[0., 0.], [1., 1.], [2., 2.]]
    y = [0, 1, 3]
    clf = MLPClassifier(solver='lbfgs', alpha=1e-5, hidden_layer_sizes=(3), random_state=1)
    clf.fit(X,y)
    test_sample = [[2., 2.], [-1., -2.]]
    print clf.predict(test_sample)
    print clf.predict_proba(test_sample)
    output_mlp(clf)

def test2():
    X = [[0., 0.], [1., 1.]]
    y = [0, 1]
    clf = MLPClassifier(solver='lbfgs', alpha=1e-5, hidden_layer_sizes=(3), random_state=1, activation='logistic')
    clf.fit(X,y)
    test_sample = [[2., 2.], [-1., -2.]]
    print clf.predict(test_sample)
    print clf.predict_proba(test_sample)
    print "coefs:"
    print [coef for coef in clf.coefs_]

#test1()
test1()
