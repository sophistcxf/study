#
g++ -g hello_world.cpp -o hello_world
objcopy --only-keep-debug hello_world hello_world.dbg
objcopy --strip-debug hello_world hello_world_strip
# 需要调试时，使用如下命令再链上符号表即可
#objcopy --add-gnu-debuglink=foo.dbg foo

objcopy -j bss hello_world hello_world hello_world_bss
