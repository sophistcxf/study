# coding=utf-8

import matplotlib.pyplot as plt

def f(x,y):
    return (x-10)**2+(y-10)**2

def f_der1(x,y):
    return [2*(x-10), 2*(y-10)]

point=[]
lossG=[]

for i in range(30):
    if i == 0:
        point.append([20,20])
    else:
        point.append([point[i-1][0]-0.1*f_der1(point[i-1][0],point[i-1][1])[0],\
                point[i-1][1]-0.1*f_der1(point[i-1][0],point[i-1][1])[1]])

    lossG.append(f(point[i][0],point[i][1]))
    print('x=',point[i][0],'y=',point[i][1], 'loss=', lossG[i])
