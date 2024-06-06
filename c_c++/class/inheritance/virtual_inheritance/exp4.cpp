#include <iostream>

class A
{
public:
    virtual void foo() { std::cout << "I'm A::foo()" << std::endl; }
public:
    int n = 1;
    unsigned long long l = 2;
};

class B1 : public A
{
public:
    virtual void foo() { std::cout << "I'm foo()" << std::endl; }
};

class B2 : public A
{
public:
    virtual void foo() {}
};

class C : public B1, public B2 {
    public:
        virtual void foo() {}
};

int main()
{
    A* p = new C();
    return 0;
}


