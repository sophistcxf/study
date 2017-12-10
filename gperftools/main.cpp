/*************************************************************************
  > File Name: main.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sun 17 Sep 2017 10:28:59 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

class A
{
public:
    char* p;
};

char* g;
A* g_a;

void test1()
{
    g = new char[4* 1024 * 1024];
}

void test2()
{
    g_a = new A;
    g_a->p = new char[3*1024*1024];
}

int main()
{
    char* str = new char[2* 1024 * 1024];
    test1();
    test2();
    g = NULL;
    str = NULL;
    g_a = NULL;
    return 0;
}
