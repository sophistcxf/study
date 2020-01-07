# coding=utf-8

import subprocess

'''
测试子进程 timeout 的场景
'''

proc = subprocess.Popen(['timeout', '5', 'python', 'inner_proc.py'], stdin=subprocess.PIPE, stdout=subprocess.PIPE)

stdout_str = proc.communicate()[0]

print stdout_str
