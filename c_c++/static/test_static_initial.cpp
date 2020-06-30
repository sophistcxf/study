/*************************************************************************
  > File Name: test_static_initial.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sat Mar 28 09:05:13 2020
 ************************************************************************/

#include <iostream>
using namespace std;

class A
{
public:
    A(int n) : value(n) { std::cout << "A " << value << " construct" << std::endl; }
    int value;
};

static A a(1);

void test1()
{
    static A a1(2);
}

int main()
{
    std::cout << "main" << std::endl;
    test1();
    test1();
    return 0;
}
