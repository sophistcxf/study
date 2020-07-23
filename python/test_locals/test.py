#! coding=utf-8

'''
Update and return a dictionary containing the current scope's local variables

什么是 scope？
'''

print locals()

a = 10
b = 'hello world'

print locals()

def foo():
    c = 20
    print locals()

foo()
