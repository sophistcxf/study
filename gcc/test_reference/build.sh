#########################################################################
# File Name: build.sh
# Author: xiangfei.cui
# mail: xiangfei.cui@alibaba-inc.com
# Created Time: Wed 22 Jul 2020 11:34:51 AM CST
#########################################################################
#!/bin/bash

g++ test1.cpp -o a1.out

g++ -O2 test1.cpp -o a2.out

g++ -O3 test1.cpp -o a3.out

g++ -g test1.cpp -o ag.out
