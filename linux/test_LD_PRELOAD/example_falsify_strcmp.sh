#########################################################################
# File Name: example_falsify_strcmp.sh
# Author: cxxxxf
# mail: sophistcxf@gmail.com
# Created Time: Sat 06 Aug 2016 05:38:33 PM CST
#########################################################################
#!/bin/bash

# 编译动态库 falsify_strcmp, 在此 strcmp 中不做任何事
# 仅输出 "falsify_strcmp"
g++ -shared -fPIC falsify_strcmp.cpp -o falsify_strcmp.so


g++ -o example_falsify_strcmp example_falsify_strcmp.cpp

# 
./example_falsify_strcmp password
./example_falsify_strcmp adf
LD_PRELOAD=./falsify_strcmp.so ./example_falsify_strcmp adf
