# coding=utf-8

'''
测试当子进程执行超过指定时间仍未完成时，则停止子进程
'''

from multiprocessing import Process
import sys
import time
import os

def sleep(sec):
    print >> sys.stderr, 'sleep %d secs' % sec
    #time.sleep(sec)
    os.system('sleep %d' % sec)

def test():
    p = Process(target=sleep, args=(5,))
    p.daemon = True
    p.start()
    print 'pid: ', p.pid
    p.join(2)
    if p.is_alive():
        print >> sys.stderr, 'timeout'
        '''
        p.terminate() 并不会使进程立即结束
        '''
        p.terminate()
        print 'p.exidcode after p.terminate() is ', p.exitcode # p.exitcode is None
        time.sleep(1)
        print 'p.exidcode after p.terminate() 1 sec later is ', p.exitcode # p.exitcode is -15
    else:
        print 'p.exitcode is ', p.exitcode

test()
