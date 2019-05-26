/*************************************************************************
  > File Name: test.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sun May 26 05:59:23 2019
 ************************************************************************/

#include <iostream>
using namespace std;

class C1
{
public:
    virtual void f1()
    {
        std::cout << "I'm C1::f1()" << std::endl;
    }
    virtual void f2()
    {
        std::cout << "I'm C1::f2()" << std::endl;
    }
};

class C2 : public C1
{
public:
    virtual void f1()
    {
        std::cout << "I'm C2::f1()" << std::endl;
    }
    virtual void f2()
    {
        std::cout << "I'm C2::f2()" << std::endl;
    }
};


void test1()
{
    C1* p = new C2();
    p->f1();
    p->C1::f1();
}


void test2()
{
    typedef void(*Fun)(void);
    Fun p_fun = NULL;
    C2 c2;
    p_fun = (Fun)*((int*)*(int*)((int*)&c2+0)+0);
    p_fun();
    p_fun = (Fun)*((int*)*(int*)((int*)&c2+1)+0);
    p_fun();
}

int main()
{
    //test1();
    test2();
    return 0;
}
