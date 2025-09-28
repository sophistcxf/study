'''
在线性回归时，是计算 y_test 与 y_label 的误差，而不是计算 (xi, yi) 
到直线的距离
'''

import numpy as np
import matplotlib.pyplot as plt

# creat test data===========================================
# data0: random in [-1,1]x[-1,1]
data0 = np.random.rand(1000,2)
data0 = 2*data0-1
# data1: data0 cutted in ellipse
data1 = np.empty((1000,2))
count = 0
for i in range(np.size(data0,0)):
    if data0[i,0]**2+4.*data0[i,1]**2<1:
        data1[count,:] = data0[i,:]
        count = count+1
data1 = data1[:count,:]
# data: data1 rotation and displacement
theta = np.pi/5
rotMatrix = np.array([[np.cos(theta),np.sin(theta)],
                      [-np.sin(theta),np.cos(theta)]])
data = np.dot(data1,rotMatrix)
data[:,0] += 2
data[:,1] += 3
# Least square Method======================================
N = np.size(data,0) # total number of data
coeMatrix = np.vstack((data[:,0],np.ones(N))).transpose()
coeRhs = data[:,1]
A = np.dot(coeMatrix.transpose(),coeMatrix)
f = np.dot(coeMatrix.transpose(),coeRhs)
kb = np.linalg.solve(A, f)
k = kb[0]
b = kb[1]
# data for plot
x_ls = np.linspace(1,3)
y_ls = x_ls*k+b
# PCA method===============================================
# move center to zero point
dataHomo = data.copy()
dataHomo[:,0] -= np.sum(data[:,0])/N
dataHomo[:,1] -= np.sum(data[:,1])/N
# data matrix
dataMatrix = np.dot(dataHomo.transpose(),dataHomo)
u, s, vh = np.linalg.svd(dataMatrix, full_matrices=True)
n = u[:,-1]
k2 = -n[0]/n[1]
b2 = np.sum(data[:,1])/N-k2*np.sum(data[:,0])/N
# data for plot
x_pca = np.linspace(1,3)
y_pca = x_pca*k2+b2


plt.plot(data[:,0],data[:,1],'.')
plt.legend('data', shadow=True,)
plt.plot(x_ls,y_ls,linewidth=3)
plt.legend('least square', shadow=True,)
plt.plot(x_pca,y_pca,linewidth=3)
plt.legend(('data', 'least square','pca'), shadow=True,)

plt.axis('equal')
plt.show()
