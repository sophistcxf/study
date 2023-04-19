import tensorflow as tf

pos = tf.placeholder(tf.float32, [1, 2], name='pos')
azi = tf.placeholder(tf.float32, [1], name='azi')
prj_pos = tf.placeholder(tf.float32, [1, 2], name='prj_pos')
link_azi = tf.placeholder(tf.float32, [1], name='link_azi')

dist_error = tf.sqrt(tf.reduce_sum(tf.square(pos, prj_pos)))
is_yaw = tf.where(dist_error > 60, 1, 0)
