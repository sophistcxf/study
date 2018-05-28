#-*- coding=utf-8 -*-
import tensorflow as tf


def queue_runner():
    files = tf.train.match_filenames_once("/tmp/log*", "pattern1")

    t = tf.train.string_input_producer(files)

    options = tf.python_io.TFRecordOptions(compression_type=tf.python_io.TFRecordCompressionType.GZIP)

def read_csv_files():
    filename_queue = tf.train.string_input_producer(["file0.csv", "file1.csv"])
    reader = tf.TextLineReader()
    key, value = reader.read(filename_queue)
    print(key)
    print(value)
    record_defaults = [[0], [0], [0], [0], [0]]
    col1, col2, col3, col4, col5 = tf.decode_csv(value, record_defaults=record_defaults)
    features = tf.stack([col1, col2, col3, col4])
    with tf.Session() as sess:
        # Start populating the filename queue.
        coord = tf.train.Coordinator()
        threads = tf.train.start_queue_runners(coord=coord)
        for i in range(1200):
            # Retrieve a single instance:
            print("index: %d" % i)
            example, label = sess.run([features, col5])
            print(example, label)

        coord.request_stop()
        coord.join(threads)

def read_csv_files2():
    '''
    与read_csv_files区别是不用start_queue_runners，start_queue_runners启动queue runners collected in the graph，
    默认是tf.GraphKeys.QUEUE_RUNNERS
    '''
    filename_queue = tf.train.string_input_producer(["file0.csv", "file1.csv"])
    reader = tf.TextLineReader()
    key, value = reader.read(filename_queue)
    print(key)
    print(value)
    record_defaults = [[0], [0], [0], [0], [0]]
    col1, col2, col3, col4, col5 = tf.decode_csv(value, record_defaults=record_defaults)
    features = tf.stack([col1, col2, col3, col4])
    with tf.Session() as sess:
        coord = tf.train.Coordinator()
        queues = tf.get_collection(tf.GraphKeys.QUEUE_RUNNERS)
        # 直接使用create_threads启动进程
        threads = []
        for queue in queues:
            print("start queue name %s" % queue.name) # queue.name is "input_producer"
            threads.extend(queue.create_threads(sess, coord, start=True))

        for i in range(1200):
            # Retrieve a single instance:
            print("index: %d" % i)
            example, label = sess.run([features, col5])
            print(example, label)

        coord.request_stop()
        coord.join(threads)

def test_matching_files():
    tf.enable_eager_execution()
    tables = tf.matching_files("file*")
    print(tables)

#read_csv_files()
read_csv_files2()
#test_matching_files()
