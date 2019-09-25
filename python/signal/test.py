# coding=utf-8


import sys
import time
import os
from multiprocessing import Process
import signal
import traceback

def signal_action(a, b):
    print 'traceback:'
    traceback.print_stack()

def sleep(sec):
    signal.signal(signal.SIGTERM, signal_action)
    print >> sys.stderr, 'sleep %d secs' % sec
    #time.sleep(sec)
    os.system('sleep %d' % sec)

def test():
    p = Process(target=sleep, args=(5,))
    p.daemon = True
    p.start()
    p.join(2)
    if p.is_alive():
        os.kill(p.pid, signal.SIGTERM)

test()
