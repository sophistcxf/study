#-*- coding=utf-8 -*-

import tensorflow as tf
import time
import threading

#tf.enable_eager_execution()

tf.InteractiveSession()

def test_queue():
    q = tf.FIFOQueue(2, "float")
    init = q.enqueue_many(([0,0],))

    # x是从q中dequeue拿到的值
    x = q.dequeue()
    y = x + 1
    q_inc = q.enqueue([y])

    init.run()
    #q_inc.run()
    #q_inc.run()
    #q_inc.run()

    print(x.eval())
    print(x.eval())
    print(x.eval())

def test_queue_runner():
    q = tf.FIFOQueue(10, "float")
    counter = tf.Variable(0.0)
    increment_op = tf.assign_add(counter, 1.0)
    enqueue_op = q.enqueue(counter)

    qr = tf.train.QueueRunner(q, enqueue_ops = [increment_op, enqueue_op] * 2)

    sess = tf.InteractiveSession()
    tf.global_variables_initializer().run()
    qr.create_threads(sess, start=True)
    # 在第一次20个输出完后，enqueue_op会阻塞住，但increment_op继续执行，也就是说counter会继续增大
    for i in range(20):
        print(sess.run(q.dequeue()))
    time.sleep(5)
    # 第二次输出时，由于sleep了10秒，输出的值会比第一次大很多
    for i in range(10):
        print(sess.run(q.dequeue()))

def test_coordinator():
    # Coordinator本身可以与python的线程一起使用
    def loop(coord, id):
        t = 0
        while not coord.should_stop():
            print("%d,%d" % (id, t))
            time.sleep(1)
            t += 1
            # 只有1号线程调用request_stop方法
            if (t >= 2 and id == 1):
                coord.request_stop()

    coord = tf.train.Coordinator()
    # 使用Python API创建10个线程
    threads = [threading.Thread(target=loop, args=(coord, i)) for i in range(10)]

    for t in threads: t.start()
    coord.join(threads)

#test_queue()
#test_queue_runner()
test_coordinator()
