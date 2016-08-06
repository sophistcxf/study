#########################################################################
# File Name: example_falsify_printf.sh
# Author: cxxxxf
# mail: sophistcxf@gmail.com
# Created Time: Sat 06 Aug 2016 05:43:41 PM CST
#########################################################################
#!/bin/bash

g++ example_falsify_printf.cpp -o example_falsify_printf
g++ -shared -fPIC -o falsify_printf.so falsify_printf.cpp
./example_falsify_printf
LD_PRELOAD=./falsify_printf.so ./example_falsify_printf
