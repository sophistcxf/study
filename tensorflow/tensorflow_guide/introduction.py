#-*- coding=utf-8 -*-
import tensorflow as tf

def Concept():
    a = tf.constant(3.0, dtype=tf.float32)
    b = tf.constant(4.0) # also tf.float32 implicitly
    total = a + b
    print(a)
    print(b)
    print(total)

    sess = tf.Session()
    print(sess.run(total))

    print(sess.run({'ab':(a,b), 'total':total}))

    '''
    一次run调用中，tf.Tensor只有一个值
    如下例，graph1和graph2是分别两个图，进行了两次计算，虽然都是Tensor vec，但其结果不同，进行了两次run调用；
    graph3在一次run调用中，vec只有一个值，因而对于out1和out2来说vec是相同的，因而out1与out2各元素的差是1
    '''
    vec = tf.random_uniform(shape=(3,))
    out1 = vec + 1
    out2 = vec + 2
    print(sess.run(vec))    # graph1
    print(sess.run(vec))    # graph2
    print(sess.run((out1, out2)))   # graph3

    '''
    Feeding
    可以使用占位符，用起来类似于函数参数。使用参数feed_dict填充占位符。
    graph中的所有Tensor都可以用feed_dict填充，区别是占位符如果不填充将报错
    '''
    x = tf.placeholder(tf.float32)
    y = tf.placeholder(tf.float32)
    k = tf.constant(10.0)
    z1 = x + y
    z = x + y + k
    print(sess.run(z, feed_dict={x:3, y:4.5}))
    print(sess.run(z, feed_dict={x:3, y:4.5, k:20.0}))
    print(sess.run(z1, feed_dict={x:[1,3], y:[2,4]}))

def SimpleTraining():
    x = tf.constant([[1], [2], [3], [4]], dtype=tf.float32)
    y_true = tf.constant([[0], [-1], [-2], [-3]], dtype=tf.float32)

    linear_model = tf.layers.Dense(units=1)
    y_pred = linear_model(x)

    sess = tf.Session()
    init = tf.global_variables_initializer()
    sess.run(init)
    print(sess.run(y_pred))

    # 使用mean square error作为loss
    loss = tf.losses.mean_squared_error(labels=y_true, predictions=y_pred)
    # 当前模型的误差
    print(sess.run(loss))

    optimizer = tf.train.GradientDescentOptimizer(0.01)
    train = optimizer.minimize(loss)

    for i in range(1000):
        # train is an op, not a tensor, it doesn't return a value when run
        _, loss_value = sess.run((train, loss))
        print(loss_value)
    
    print(sess.run(y_pred))

def Tensor():
    sess = tf.Session()
    my_image = tf.zeros([10, 299, 299, 3])
    r = tf.rank(my_image)
    print(sess.run(r))

    # referring to tf.Tensor slices
    slice1 = my_image[1]
    print(slice1)

    s = tf.shape(my_image)
    print(sess.run(s))

    # Tensor.eval()需要一个default session才能运行
    constant = tf.constant([1,2,3])
    tensor = constant * constant
    print tensor.eval()

def Variable():
    my_variable = tf.get_variable("my_variable", [1, 2, 3])

def SaveRestore():
    # Create some variables.
    v1 = tf.get_variable("v1", shape=[3], initializer = tf.zeros_initializer)
    v2 = tf.get_variable("v2", shape=[5], initializer = tf.zeros_initializer)

    inc_v1 = v1.assign(v1+1)
    dec_v2 = v2.assign(v2-1)

    init_op = tf.global_variables_initializer()

    saver = tf.train.Saver()

    with tf.Session() as sess:
        sess.run(init_op)
        inc_v1.op.run()
        dec_v2.op.run()
        # 没有/tmp/model.ckpt这个文件，/tmp/model.ckpt只是个前缀
        save_path = saver.save(sess, "/tmp/model.ckpt")
        print("Model saved in path: %s" % save_path)

    tf.reset_default_graph()
    v1 = tf.get_variable("v1", shape=[3])
    v2 = tf.get_variable("v2", shape=[5])
    saver = tf.train.Saver()
    with tf.Session() as sess:
        saver.restore(sess, "/tmp/model.ckpt")
        print("Model restored.")
        print("v1 : %s" % v1.eval())
        print("v2 : %s" % v2.eval())


#SimpleTraining()
#Tensor()
#Variable()
SaveRestore()
