#include <iostream>

class A
{
public:
    virtual void foo() { std::cout << "This is A::foo" << std::endl; }
};

class B : public A
{
public:
    virtual void foo() { std::cout << "This is B::foo" << std::endl; }
};

class B2 : public A
{
    virtual void foo() { std::cout << "This is B2::foo" << std::endl; }
};

class F : public B
{
};

/**
 * 第二种类，没有继承关系的非多态类，没有虚函数
 */
class C
{
};

class D : public C
{
};

/**
 * 第三种类，没有继承关系的多态类
 */
class E
{
public:
    virtual void foo() { std::cout << "I'm E::foo" << std::endl; }
};

class G
{
public:
    virtual void foo() = 0;
};

class H : public G
{
public:
    virtual void foo() {}
};

void test1()
{
    A* pa = new A();
    A* pb = new B();
    A* pb2 = new B2();
    C* pc = new C();
    D* pd = new D();
    E* pe = new E();

    A* p;
    p = pb;
    p = static_cast<A*>(pb);
    p = dynamic_cast<A*>(pb);

    // static_cast from 'C *' to 'A *', which are not related by inheritance, is not allowed
    // C 与 A 没有继承关系，不能转换
    // p = static_cast<A*>(pc);

    // compile OK
    // 这里 E 与 A 没有继承关系，但因为是多态，也可以通过编译，也可以转换
    p = dynamic_cast<A*>(pe);
    if (p != NULL) {
        p->foo();
    }
    else {
        std::cout << "p = dynamic_cast<A*>(pe); " << "is NULL" << std::endl;
    }

    // compile OK
    p = reinterpret_cast<A*>(pc);

    // error: 'E' is not polymorphic
    // E 不是多态，不能转
    //p = dynamic_cast<A*>(pe);

    p = reinterpret_cast<A*>(pe);

    B* p2;

    p2 = static_cast<B*>(pb);
    p2 = static_cast<B*>(pb2);
    if (p2 != NULL) {
        p2->foo();
    }
    else {
        std::cout << "p2 = static_cast<B*>(pb2); " << "is NULL" << std::endl;
    }

    p2 = dynamic_cast<B*>(pb2);
    if (p2 != NULL) {
        p2->foo();
    }
    else {
        // do this
        std::cout << "p2 = dynamic_cast<B*>(pb2); " << "is NULL" << std::endl;
    }
    

    //p2 = pa;  // error: assigning to 'B *' from incompatible type 'A *'
    //p2 = static_cast<B*>(pa);


    // error: error: 'C' is not polymorphic
    // dynamic_cast 用于 downcast 时，一定要是多态类
    C* p3;
    p3 = dynamic_cast<C*>(pd);
}

void test2()
{
    G* p = new H();

    H* p2 = dynamic_cast<H*>(p);
    p2 = static_cast<H*>(p);

    G& r = *p;
    H& r2 = dynamic_cast<H&>(r);
    H& r3 = static_cast<H&>(r);
}

int main()
{
    //test1();
    test2();
    return 0;
}
