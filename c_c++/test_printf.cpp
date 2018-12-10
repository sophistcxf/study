/*************************************************************************
  > File Name: test_printf.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sun 14 Oct 2018 06:41:30 PM CST
 ************************************************************************/

/*!
 * 测试 printf 函数族
 * printf
 * fprintf
 * sprintf
 * snprintf
 * vprintf
 * vfprintf
 * vsprintf
 * vsnprintf
 */

#include <iostream>
#include <stdio.h>
using namespace std;

void test_precision()
{
    int num = 4, width = 3;
    
    /* width 指定了宽度，*/
    printf("%*d,%*d\n", width, num, num, width);
    printf("%2$*1$d\n", width, num);
    
    printf("%.0d\n", num);
    printf("%.3d\n", num);

    /* 对于s,S来说，precision 控制打印出多少个字符 */
    const char* str = "hello world!";
    printf("%.3s\n", str);
}

void test_field_width()
{
    int num = 4;
    printf("%2d,%4.3d\n", num, num);
    
    float f = 1.234;
    printf("%6f, %7.6f\n", f, f);
}

int main(int argc, char* argv[])
{
    test_precision();
    test_field_width();
    return 0;
}
