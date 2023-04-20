import tensorflow.compat.v1  as tf
import numpy as np

tf.disable_eager_execution()



graph=tf.Graph()

with tf.Session(graph=graph) as sess:
    pos = tf.placeholder(tf.float32, [2, 1], name='pos')
    prj_pos = tf.placeholder(tf.float32, [2, 1], name='prj_pos')
    dist_error = tf.sqrt(tf.reduce_sum(tf.square(pos - prj_pos)))
    dist_error = tf.abs(dist_error, name='dist_error')
    is_yaw = tf.where(dist_error > 60, 1, 0, name='is_yaw')
    names = ['dist_error','is_yaw']
    pb_graph = tf.graph_util.convert_variables_to_constants(sess, graph.as_graph_def(), names)

    with tf.gfile.FastGFile('saved_model.pb', mode='wb') as f:
        f.write(pb_graph.SerializeToString())

    
    ret = sess.run(is_yaw, feed_dict={pos:np.array([[1.0], [2.0]]), prj_pos:np.array([[3.0], [1000.0]])})
    print (ret)