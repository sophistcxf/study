#! -*- coding=utf-8 -*-

import traceback

def simple_test():
    def foo():
      yield 1
      yield 2
      yield 3
    p = foo()
    print p
    print p.next()
    print p.next()
    print p.next()
    try:
      print p.next()  # exception, StopIteration
    except Exception:
      print traceback.print_exc()
    # 遍历生成器，什么也不打
    for i in p:
        print i
    p = foo()
    # 遍历生成器，如期打印结果
    for i in p:
        print i

def endless_generate_object():
    def foo():
        number = 0
        while True:
            yield number
            number += 1

    p = foo()
    for i in range(0, 100):
        print p.next() 
    for i in p:
        print i
  
def generate_obj_send():
    def foo():
        number = 0
        while True:
            # 没有调用 send 方法时，val 是 None，当调用 send 方法时，val 是 send 方法传入的参数
            # 即只有 send 的参数会打印出 val: val 这行日志
            val = yield number
            if val:
              print "val: ", val
              number = val
            else:
              number += 1
    p = foo()
    for i in range(5):
        print p.next()
    print p.send(100)
    print p.send(200)
    for i in range(5):
        print p.next()

#simple_test()
endless_generate_object()
#generate_obj_send()
