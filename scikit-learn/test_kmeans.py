#-*- coding=utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt
import math

from sklearn.cluster import KMeans


def test1():
    X = np.array([[1, 2], [1, 0], [4, 2], [1, 4], [4, 4], [4, 0]])
    kmeans = KMeans(n_clusters=2, random_state=0)
    # 调用KMeans之后还没有labels_成员变量，要调用完fit之后才有
    fitted = kmeans.fit(X)

    # KMeans的三个成员
    print "cluster centers:\n", fitted.cluster_centers_
    print "labels:\n", fitted.labels_
    print "inertia:\n", fitted.inertia_

    # inertia_即是每个点与其聚类中心的距离的平方和
    '''
    dist_sum = 0.0
    for i in range(0, len(X)):
        dist_sum += (pow(X[i][0] - fitted.cluster_centers_[fitted.labels_[i]][0],2) + pow(X[i][1]-fitted.cluster_centers_[fitted.labels_[i]][1],2))
    print "dist_sum=", dist_sum
    # will be True
    print abs(fitted.inertia_-dist_sum) < 0.000001
    '''

    predicted = fitted.predict(X)
    print "predicted:\n", predicted

    # transform返回的是每个样本与每个聚类中心的距离
    # 维度为N*K，N为样本的个数，K为类别
    transformed = fitted.transform(X)
    print "transformed:\n", transformed

    # 根据transforme计算inertia
    '''
    dist_sum = 0.0
    for i in range(0, len(transformed)):
        dist_sum += pow(min(transformed[i]), 2)
    print "dist_sum=", dist_sum
    print abs(fitted.inertia_-dist_sum) < 0.000001
    '''

if __name__ == "__main__":
    test1()
    pass
