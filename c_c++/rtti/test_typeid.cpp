#include <iostream>
#include <typeinfo>

class A
{
public:
    virtual void foo() {}
};

class B : public A
{
    virtual void foo() {}
};

class C
{
};

class D : public C
{
};

void test1()
{
    A* pa = new A();
    A* pb = new B();

    // error: calling a private constructor of class 'std::type_info'
    //const std::type_info ti = typeid(A);
    
    const std::type_info& ti = typeid(A);

    std::cout << typeid(pa).name() << std::endl;
    std::cout << typeid(pb).name() << std::endl;
    std::cout << typeid(*pa).name() << std::endl;
    std::cout << typeid(*pb).name() << std::endl;

    // typeid 依赖于多态，如下代码，输出的全是 C 的信息
    C* pc = new C();
    C* pd = new D();
    std::cout << typeid(pc).name() << std::endl;
    std::cout << typeid(pd).name() << std::endl;
    std::cout << typeid(*pc).name() << std::endl;
    std::cout << typeid(*pd).name() << std::endl;
}

int main()
{
    test1();
    return 0;
}
