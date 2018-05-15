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
    record_defaults = [[1], [1], [1], [1], [1]]
    col1, col2, col3, col4, col5 = tf.decode_csv(value, record_defaults=record_defaults)
    features = tf.stack([col1, col2, col3, col4])
    with tf.Session() as sess:
        # Start populating the filename queue.
        coord = tf.train.Coordinator()
        threads = tf.train.start_queue_runners(coord=coord)
        for i in range(1200):
            # Retrieve a single instance:
            example, label = sess.run([features, col5])

        coord.request_stop()
        coord.join(threads)

read_csv_files()
