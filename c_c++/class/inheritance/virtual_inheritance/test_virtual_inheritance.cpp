#include <iostream>

class A
{
public:
    virtual void foo() { std::cout << "I'm A::foo()" << std::endl; }
public:
    int n;
    unsigned long long l;
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

class C : public B1, public B2
{
};

void test1()
{
    std::cout << sizeof(A) << "\t" << sizeof(B1) << "\t" << sizeof(B2) << std::endl;
}

void test2()
{
    std::cout << "sizeof(A): " << "\t" << sizeof(A) << std::endl
              << "sizeof(B1): " << "\t" << sizeof(B1) << std::endl
              << "sizeof(B2): " << "\t" << sizeof(B2) << std::endl;

    A* pa = new A();
    std::cout << "pa: " << pa << std::endl
              << "&(pa->n): " << &(pa->n) << std::endl
              << "&(pa->l): " << &(pa->l) << std::endl;

    A* p = new B1();
    // error: cannot cast 'A *' to 'B1 *' via virtual base 'A'
    //B1* p1 = static_cast<B1*>(p);
    B1* p1 = dynamic_cast<B1*>(p);
    // p 与 p1 不相等，p 比 p1 多 8 字节
    // 内存布局是 vptr_b1, vptr_a, n
    std::cout << "p: " << p << std::endl 
              << "p1: " << p1 << std::endl
              << "&(p->n): " << &(p->n) << std::endl
              << "&(p1->n): " << &(p->n) << std::endl
              << "&(p->l): " << &(p->l) << std::endl;
    
    p = new B2();
    // OK
    B2* p2 = static_cast<B2*>(p);
    // p 与 p2 是相等的
    std::cout << p << "\t" << p2 << std::endl;
}

void test3()
{
    A* pb1 = new B1();
    A* pc = new C();
}

int main()
{
    //test2();
    test3();
    return 0;
}
