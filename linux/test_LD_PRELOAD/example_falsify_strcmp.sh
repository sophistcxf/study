#########################################################################
# File Name: example_falsify_strcmp.sh
# Author: cxxxxf
# mail: sophistcxf@gmail.com
# Created Time: Sat 06 Aug 2016 05:38:33 PM CST
#########################################################################
#!/bin/bash

g++ -shared -fPIC falsify_strcmp.cpp -o falsify_strcmp.so
g++ -o example_falsify_strcmp example_falsify_strcmp.cpp
./example_falsify_strcmp password
./example_falsify_strcmp adf
LD_PRELOAD=./falsify_strcmp.so ./example_falsify_strcmp adf
