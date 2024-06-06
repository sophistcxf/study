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
    virtual void foo() { std::cout << "I'm B1::foo()" << std::endl; }
};

class B2 : public virtual A
{
public:
    virtual void foo() { std::cout << "I'm B2::foo()" << std::endl; }
};

class C : public B1, public B2 {
};

int main()
{
    A* p = new C();
    return 0;
}


