/*************************************************************************
  > File Name: test_union.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Wed 03 Aug 2016 02:05:32 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

struct C
{
    short ui;
};

union
{
 int ui;
 char c[4];
 bool ub;
 C struct_c;
} u1;

class A
{
public:
    //union member 'a' has a non-trivial default constructor
    //A() : value(0) {} A用在union中时，不能自定义构造函数
    void foo() {}
public:
    int value;
    char* buf;
};

union
{
    int ui;
    A a;
} u2;

void test1()
{
  cout << sizeof(u1) << endl;   // 4
  u1.ui = 0x41ffff42;
  cout << u1.ui << endl;    // 1107296066
  cout << u1.ub << endl;    // 0
  cout << u1.c[0] << endl;  // 'B'
  cout << u1.struct_c.ui << endl;   // -190
}

void test2()
{
    u2.a = A();
}

int main()
{
    test2();
    return 0;
}
