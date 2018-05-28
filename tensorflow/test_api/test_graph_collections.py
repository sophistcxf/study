import tensorflow as tf

tf.enable_eager_execution()

def test_get_collection():
    queue = tf.get_collection(tf.GraphKeys.QUEUE_RUNNERS)
    print(queue)

test_get_collection()
