#-*- coding=utf-8 -*-

import tensorflow as tf

def test_default_graph():
    # 如果不指定Graph，定义一个新的Operation时就是加到default graph上
    old_default_graph = tf.get_default_graph()
    c = tf.constant(4)
    assert c.graph is tf.get_default_graph()

    # 可以覆盖掉默认的Graph
    g = tf.Graph()
    with g.as_default():
        c = tf.constant(30.0)
        assert c.graph is g
        # assert c.graph is old_default_graph # 将断言失败
    
    c = tf.constant(5)
    c.graph is old_default_graph    # 出了作用域后，default_graph恢复，因而此断言成功

    v = tf.Variable(10)
    v2 = tf.Variable(20)

    # Variable会放入到名为tf.GraphKeys.GLOBAL_VARIABLES的collection中
    print(tf.get_collection(tf.GraphKeys.GLOBAL_VARIABLES))

test_default_graph()
