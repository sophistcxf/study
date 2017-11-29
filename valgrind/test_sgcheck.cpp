/*************************************************************************
  > File Name: test_sgcheck.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 29 Nov 2017 10:43:17 AM CST
 ************************************************************************/

/*!
 *exp-sgcheck与memcheck不同，memcheck检测的是heap，sgcheck检测的是stack和global arrays
 *所以本例中栈上数组a的写越界memcheck就检测不出来，sgcheck可以检测出来
 *http://valgrind.org/docs/manual/sg-manual.html#sg-manual.cmp-w-memcheck
 */

#include <iostream>
using namespace std;

int main()
{
    int i, a[10];
    for (i = 0; i <= 10; ++i)
    {
        a[i] = 10;
    }
    return 0;
}
