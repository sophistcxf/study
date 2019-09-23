# coding=utf-8

'''
测试当子进程执行超过指定时间仍未完成时，则停止子进程
'''

from multiprocessing import Process
import sys
import time

def sleep(sec):
    print >> sys.stderr, 'sleep %d secs' % sec
    time.sleep(sec)

def test():
    p = Process(target=sleep, args=(5,))
    p.daemon = True
    p.start()
    print 'pid: ', p.pid
    p.join(1)
    if p.is_alive():
        print >> sys.stderr, 'timeout'
        p.terminate()

test()
