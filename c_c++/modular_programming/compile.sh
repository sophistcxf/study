gcc -g -shared -o van.so -fpic van.c
gcc -rdynamic -o car main.c -ldl
