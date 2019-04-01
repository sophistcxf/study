#! -*- coding=utf-8 -*-

from sklearn import metrics
import math

pred_probs = [[[.1, .9], [.9, .1], [.8, .2], [.35, .65]],
              [[.05, .95], [.9, .1], [.8, .2], [.35, .65]],
              [[.05, .95], [.95, .05], [.8, .2], [.35, .65]]]

'''
计算交叉熵
-(log(0.9))+(-log(0.9))+(-log(0.8))+(-log(0.65))
'''
print metrics.log_loss(["2", "1", "1", "2"],
        [[.1, .9], [.9, .1], [.8, .2], [.35, .65]])

# metrics.log_loss 做了 normalize，即除以 4
print (-(math.log(0.9))+(-math.log(0.9))+(-math.log(0.8))+(-math.log(0.65)))/4.0

# 对比三种分类的交叉熵
for i in pred_probs:
        print metrics.log_loss(["2", "1", "1", "2"], i)