#include <iostream>

class A
{
public:
    virtual void foo() { std::cout << "I'm A::foo()" << std::endl; }
public:
    int n = 1;
    unsigned long long l = 2;
};

class B1 : public virtual A
{
public:
    virtual void foo() { std::cout << "I'm foo()" << std::endl; }
};

class B2 : public A
{
public:
    virtual void foo() {}
};

int main()
{
    std::cout << sizeof(A) << "\t" << sizeof(B1) << "\t" << sizeof(B2) << std::endl;
    return 0;
}


