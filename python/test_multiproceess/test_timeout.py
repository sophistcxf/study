# coding=utf-8

'''
测试当子进程执行超过指定时间仍未完成时，则停止子进程
'''

from multiprocessing import Process
import sys
import time

def sleep(sec):
    print >> sys.stderr, 'sleep %d secs' % sec
    #time.sleep(sec)
    while True:
        time.sleep(1)

def test():
    p = Process(target=sleep, args=(1000,))
    p.start()
    print 'pid: ', p.pid
    p.join(1)

test()
