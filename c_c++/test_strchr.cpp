/*************************************************************************
  > File Name: test_strchr.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Thu 07 Dec 2017 11:50:25 PM CST
 ************************************************************************/

#include <iostream>
#include <string.h>
using namespace std;

int main()
{
    char* p = "1952$abcdef";
    if (strstr(p, "19522$") == p)
        std::cout << "OK\n";
}
