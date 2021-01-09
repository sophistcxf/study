#include <iostream>

class A
{
public:
    virtual void foo() {}
};

class B : public A
{
};

int main()
{
    A* pa = new A();
    std::cout << sizeof(*pa) << std::endl;
    return 0;
}
