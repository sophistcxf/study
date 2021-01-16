#include <iostream>

class A
{
public:
    virtual void foo() {}
};

class B : public A 
{
public:
    virtual void foo() {}
};

class C : public B 
{
public:
    virtual void foo() {}
};

int main()
{
    C* p = new C;
    A* p1 = p;
    B* p2 = p;
    std::cout << p << "\t" << p1 << "\t" << p2 << std::endl;
    return 0;
}
