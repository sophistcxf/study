/*************************************************************************
  > File Name: test_va_list.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Mon 20 Aug 2018 05:48:41 PM CST
 ************************************************************************/

#include <iostream>
#include <stdarg.h>
#include <stdio.h>
using namespace std;


#define PRINT(format, ...) \
    do  \
    {   \
        char buffer[256] = {0}; \
        snprintf(buffer, 255, format, ##__VA_ARGS__);   \
        printf("%s", buffer);   \
    }   \
    while (false)

void diy_printf(const char* format, ...)
{
    va_list vl;
    va_start(vl, format);
    char buffer[256] = {0};
    vsprintf(buffer, format, vl);
    printf("%s", buffer);
    va_end(vl);
}

int main(int argc, char* argv[])
{
    printf("my name is %s, my age is %d\n", "cxxxxf", 30); 
    diy_printf("my name is %s, my age is %d\n", "cxxxxf", 30); 
    PRINT("my name is %s, my age is %d\n", "cxxxxf", 30);
    return 0;
}
