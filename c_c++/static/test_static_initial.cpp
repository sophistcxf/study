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
A a3(3);

void test1()
{
    /**
     * &a1 与 &a 的接近
     * &j 比 &i 小 20 字节
     */
    int i = 0;
    static A a1(2);
    int j = 0;
    printf("%x,%x,%x,%d\n", &i, &a1, &j, (char*)&i-(char*)&j);
}

void test2()
{
    /**
     * &j 比 &i 小 4 字节
     */
    int i = 0;
    int j = 0;
    printf("%x,%x,%d\n", &i, &j, (char*)&i-(char*)&j);
}

int main()
{
    std::cout << "main" << std::endl;
    printf("%x\n", &a);
    test1();
    test1();
    test2();
    return 0;
}
