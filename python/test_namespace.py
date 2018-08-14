#-*- coding=utf-8 -*-


i = 0
def fun1():
    global i
    i = i + 1
    print i

def fun3():
    print i

def fun2():
    i = i + 1
    print i

fun1()
fun3()
fun2()
