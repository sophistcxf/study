import tensorflow as tf

def test_batch():
    t_list = []
    for i in range(10):
        c = [j * i for j in range(10)]
        t_list.append(tf.constant(c))

    #t_list = [[1,2,3,4], [5,6,7,8],[9,10,11,12],[13,14,15,16],[17,18,19,20],[21,22,23,24]]
    t_list = [tf.constant([[1,2,3,4],[4,1,2,3],[3,4,1,2]]), tf.constant([[5,6,7,8,0],[8,5,6,7,0],[7,8,5,6,0]]),tf.constant([[9,10,11,12],[12,9,10,11],[11,12,9,10]]),tf.constant([[13,14,15,16],[16,13,14,15],[15,16,13,14]]),tf.constant([[17,18,19,20],[20,17,18,19],[19,20,17,18]]),tf.constant([[21,22,23,24],[24,21,22,23],[23,24,21,22]])]
    #input_queue = tf.train.slice_input_producer(t_list, shuffle=False, num_epochs=1)

    bat_size = 4
    b = tf.train.batch(t_list, bat_size, num_threads=1, capacity=64, enqueue_many=True)

    sess = tf.Session()

    sess.run(tf.global_variables_initializer())
    sess.run(tf.local_variables_initializer())

    coord = tf.train.Coordinator()
    threads = tf.train.start_queue_runners(coord=coord, sess=sess)

    '''
    try:
        while not coord.should_stop():
            print(sess.run(b))
    except tf.errors.OutOfRangeError:
        print('Done training')
    finally:
        coord.request_stop()
    '''
    

    for i in b:
        print(sess.run(i))

    coord.request_stop()
    coord.join(threads)

test_batch()
