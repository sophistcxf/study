#########################################################################
# File Name: build.sh
# Author: xiangfei.cui
# mail: xiangfei.cui@alibaba-inc.com
# Created Time: Sun 17 Sep 2017 10:46:18 PM CST
#########################################################################
#!/bin/bash

g++ -g main.cpp libs/libtcmalloc.a -lunwind -lrt
