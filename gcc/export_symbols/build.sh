#########################################################################
# File Name: build.sh
# Author: xiangfei.cui
# mail: xiangfei.cui@alibaba-inc.com
# Created Time: Mon 12 Jul 2021 06:02:22 PM CST
#########################################################################
#!/bin/bash


# 导出所有符号
gcc -fPIC -shared -o libfoo_all_syms.so foo.c

# 导出 libfoo.version 中指定的符号
gcc -fPIC -shared -o libfoo_specified_syms.so foo.c -Wl,--version-script=libfoo.version

nm -D libfoo.so libfoo_all_syms.so
