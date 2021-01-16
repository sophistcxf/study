#include <iostream>

class A
{
public:
    virtual void foo() {}
private:
    int n;
};

class B1 : public virtual A
{
public:
    virtual void foo() {}
};

class B2 : public A
{
public:
    virtual void foo() {}
};

void test1()
{
    std::cout << sizeof(A) << "\t" << sizeof(B1) << "\t" << sizeof(B2) << std::endl;
}

void test2()
{
    A* p = new B1();
    // error: cannot cast 'A *' to 'B1 *' via virtual base 'A'
    //B1* p1 = static_cast<B1*>(p);
    B1* p1 = dynamic_cast<B1*>(p);
    // p 与 p1 不相等，p 比 p1 多 8 字节
    // 内存布局是 vptr_b1, vptr_a, n
    std::cout << p << "\t" << p1 << std::endl;
    
    p = new B2();
    // OK
    B2* p2 = static_cast<B2*>(p);
    // p 与 p2 是相等的
    std::cout << p << "\t" << p2 << std::endl;
}

int main()
{
    //test1();
    test2();
    return 0;
}
