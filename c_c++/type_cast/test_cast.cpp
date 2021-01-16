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
public:
    virtual void foo() { std::cout << "This is B2::foo" << std::endl; }
public:
    int a[100];
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

/**
 * 第四种类
 */
class G
{
public:
    virtual void foo() = 0;
    virtual void foo1() = 0;
};

class H : public G
{
public:
    virtual void foo2() = 0;
};

class I : public H
{
public:
    virtual void foo() {}
    virtual void foo1() {}
    virtual void foo2() {}
};

/**
 * 第五种类
 */
class J
{
public:
    virtual void foo() {}
};

class K : virtual public J
{
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
    G* p = new I();

    H* p2 = dynamic_cast<H*>(p);
    p2 = static_cast<H*>(p);

    // 基类转为派生类
    H* p3 = dynamic_cast<H*>(p);
    p3 = static_cast<H*>(p);
    
    G* p4 = dynamic_cast<G*>(p3);

    G& r = *p;
    H& r2 = dynamic_cast<H&>(r);
    H& r3 = static_cast<H&>(r);
}

void test3()
{
    J* p = new K();

    K* p1 = dynamic_cast<K*>(p);

    // error: cannot cast 'J *' to 'K *' via virtual base 'J'
    // 由于 K 是虚继承于 J，所以对象*p 实际上没有存储基类 J 的实例
    // 而是存储了一个指向 J 实例的指针（所有 K 的实例都是这样），
    // 而这个地址在编译期是无法确定的，所以不能使用 static_cast
    //p1 = static_cast<K*>(p);
}

void test4()
{
    int temp = 0;
    B b;
    A* p = &b;
    B2* p2 = static_cast<B2*>(p);
    std::cout << "各变量地址: " << std::endl;
    std::cout << "&temp: " << &temp << std::endl
              << "&b: " << &b << std::endl
              << "&p: " << &p << std::endl
              << "&p2: " << &p2 << std::endl
              << "&p2->a[0]: " << &(p2->a[0]) << std::endl
              << "&p2->a[0]: " << &(p2->a[1]) << std::endl;
    std::cout << "sizeof(B): " << sizeof(B) << std::endl;
    // 这里调用的是 B::foo()
    p2->foo();
    p2->a[0] = 10;
    p2->a[1] = 11;
    std::cout << p2->a[0] << std::endl;
    // 对 a[1] 的赋值污染了 temp
    std::cout << temp << std::endl;
}

int main()
{
    //test1();
    //test2();
    //test3();
    test4();
    return 0;
}
