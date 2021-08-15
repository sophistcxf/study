#########################################################################
# File Name: build.sh
# Author: xiangfei.cui
# mail: xiangfei.cui@alibaba-inc.com
# Created Time: Wed 02 Jun 2021 10:22:05 PM CST
#########################################################################
#!/bin/bash

g++ -I /usr/include/python3.6m/ test.cpp -o test /usr/lib64/libpython3.6m.so
