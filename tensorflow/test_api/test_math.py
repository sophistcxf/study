#-*- coding=utf-8 -*-

import tensorflow as tf


tf.enable_eager_execution()

def test_abs():
    x = tf.constant([[-2.25 + 4.75j], [-3.25 + 5.75j]])
    print(tf.abs(x))

def test_sign():
    print(tf.sign([-10]))
    print(tf.sign([-0.5]))
    print(tf.sign([0, -10, 10]))

def test_reduce_num():
    x = tf.constant([[1, 1, 1], [1, 1, 1]])
    print(tf.reduce_sum(x))
    print(tf.reduce_sum(x, 0))
    print(tf.reduce_sum(x, 1))
    print(tf.reduce_sum(x, 1, keepdims=True))
    print(tf.reduce_sum(x, [0, 1]))
    t = [[[1, 1], [2,2]], [[3,3], [4,4]]]
    print(tf.shape(t))
    print(tf.reduce_sum(t, axis=0))
    print(tf.reduce_sum(t, axis=1))
    print(tf.reduce_sum(t, axis=2))
    
def test_reshape():
    t = [1, 2, 3, 4, 5, 6, 7, 8, 9]
    print(tf.reshape(t, [3, 3]))
    try:
        print(tf.reshape(t, [4, 5]))
    except:
        print("tf.reshape(t, [4, 5]) exception")
    t = [[[1, 1], [2,2]], [[3,3], [4,4]]]
    print(tf.reshape(t, [2,4]))
    print(tf.reshape(t, [-1,8]))
    print(tf.reshape(t, [-1, 2]))
    print(tf.reshape(t, [8, -1]))

def test_multiply():
    a = [[1,2,3], [3,4,5]]
    b = [[6,7,8],[8,10,11]]
    # multiply不是矩阵乘法，而是element-wise的乘法
    # matmul是矩阵乘法
    print(tf.multiply(a,b))

def test_matmul():
    a = [[1,2,3], [3,4,5]]
    b = [[6,7],[8,10],[9,11]]
    print(tf.matmul(a,b))

def test_expand_dims():
    t = tf.constant([[2],[1]])
    print(tf.shape(t))
    print(tf.expand_dims(t, 0))
    print(tf.expand_dims(t, 1))
    print(tf.expand_dims(t,-1))
    t = tf.constant([2, 3, 5])
    print(tf.expand_dims(t, 0))
    print(tf.expand_dims(t, 1))
    print(tf.expand_dims(t, 3))
    print(tf.expand_dims(t, -1))

def test_square():
    t = [[[1, 1], [2,2]], [[3,3], [4,4]]]
    print(tf.square(t))
    print(tf.reduce_sum(t, axis=[0,1,2]))

def test_clip_by_global_norm():
    t_list = [[1., 2., 3.], [1., 2.]]
    clip_norm = 2.0
    print(tf.clip_by_global_norm(t_list, clip_norm))

test_clip_by_global_norm()
#test_abs()
#test_sign()
#test_reduce_num()
#test_reshape()
#test_multiply()
#test_matmul()
#test_expand_dims()
#test_square()
