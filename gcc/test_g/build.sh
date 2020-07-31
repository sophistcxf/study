#########################################################################
# File Name: build.sh
# Author: xiangfei.cui
# mail: xiangfei.cui@alibaba-inc.com
# Created Time: Wed 22 Jul 2020 04:02:24 PM CST
#########################################################################
#!/bin/bash

g++ -g hello_world.cpp -o a_g.out

g++ hello_world.cpp -o a.out

g++ -O2 hello_world.cpp -o a_o2.out

g++ -O3 hello_world.cpp -o a_o3.out
