# coding=utf-8

import numpy as np
import math

from scipy.spatial.transform import Rotation as R

# 绕 [0,0,1] 轴旋转 pi/2
r = R.from_quat([0, 0, np.sin(np.pi/4), np.cos(np.pi/4)])

print '用旋转矩阵表示:'
print r.as_rotvec()

print '用欧拉角表示:'
print 'zyx:'
print r.as_euler('zyx', degrees=True)   # [90, 0, 0]
print 'xyz:'
print r.as_euler('xyz', degrees=True)   # [0, 0, 90]

print '用四元数表示:'
print r.as_quat()

# 用方向余弦矩阵表示，direction consin matrices
print '方向余弦矩阵:'
print r.as_dcm()

# 把 [1, 0, 0] 绕 z 轴旋转90度，得到
print '把 [1,0,0] 绕 z 轴旋转90度:'
vector = np.array([1, 0, 0])
print r.apply(vector)
print r.apply(vector, inverse=True)

# 四元数、方向余弦矩阵、欧拉角、旋转矩阵，这四种表示是如何转化的


# 绕 [1, 1, 0] 轴旋转 pi/2
# 需要转成单位四元数，不转的话，旋转角度将不同志是 pi/2
f = math.sqrt((1 - np.cos(np.pi/4)**2) / 2.0 / np.sin(np.pi/4) ** 2)
r = R.from_quat([np.sin(np.pi/4) * f, np.sin(np.pi/4) * f, 0, np.cos(np.pi/4)])
print r.as_quat()
print r.as_euler('xzy')
vector = np.array([1, 0, 0])
print r.apply(vector)
