/*************************************************************************
  > File Name: test_alignas.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 五  1/15 22:27:15 2021
 ************************************************************************/

#include <iostream>

class A
{
    int i;
    char a;
};

class alignas(16) B
{
    int i;
    char a;
};

void test1()
{
    std::cout << sizeof(A) << std::endl;
    std::cout << sizeof(B) << std::endl;
}

int main()
{
    test1();
    return 0;
}
