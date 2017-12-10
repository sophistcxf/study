#########################################################################
# File Name: build.sh
# Author: xiangfei.cui
# mail: xiangfei.cui@alibaba-inc.com
# Created Time: Sun 17 Sep 2017 11:10:36 AM CST
#########################################################################
#!/bin/bash

set -o errexit
set -o nounset

g++ -o test -g main.cpp
# 将debug信息copy到一个文件里面
objcopy --only-keep-debug test test.dbg
# 将可执行文件中的debug信息strip掉
cp test test_strip
objcopy --strip-debug test_strip
# 将调试信息文件与执行文件链起来
objcopy --add-gnu-debuglink=test.dbg test_strip
