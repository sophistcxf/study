#########################################################################
# File Name: processor.sh
# Author: xiangfei.cui
# mail: xiangfei.cui@alibaba-inc.com
# Created Time: 二  3/13 18:01:40 2018
#########################################################################
#!/bin/bash

[ "$#" -ne 2 ] && exit 1

id=$1
t=$2

function process()
{
    local idx=$1
    local t=$2
    echo "process id "${idx}", sleep "${t}" sec"
    sleep $t 
}

process $id $t
