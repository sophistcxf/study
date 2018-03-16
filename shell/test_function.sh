#########################################################################
# File Name: test_function.sh
# Author: xiangfei.cui
# mail: xiangfei.cui@alibaba-inc.com
# Created Time: Fri 16 Mar 2018 10:36:02 AM CST
#########################################################################
#!/bin/bash

function func()
{
    sleep 20
}

func &
func &

#如果wait，则两个子test_function进程是执行shell进程的子进程
#如果不wait，则两个子test_function进程的父进程是init进程
wait
