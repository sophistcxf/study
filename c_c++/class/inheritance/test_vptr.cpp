#include <iostream>

class A
{
public:
    virtual void foo() { std::cout << "I'm A::foo" << std::endl; }
};

class B : public A
{
public:
    virtual void foo() { std::cout << "I'm B::foo" << std::endl; }
};

void test1()
{
    A* p = new B();
    p->foo();
    p + sizeof(std::type_info);
}

int main()
{
    test1();
    return 0;
}
