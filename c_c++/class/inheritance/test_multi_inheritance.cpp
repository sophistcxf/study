#include <iostream>

class X {};
class Y : public virtual X {};
class Z : public virtual X {};
class A : public Y, public Z {};

void test1()
{
    std::cout << sizeof(X) << std::endl
    << sizeof(Y) << std::endl
    << sizeof(Z) << std::endl
    << sizeof(A) << std::endl;
}

class Base1
{
public:
    Base1() {}
    virtual ~Base1() {}
    virtual void speakClearly() {}
    virtual Base1* clone() const { return NULL; }
protected:
    float data_base1;
};

class Base2
{
public:
    Base2() {}
    virtual void mumble() {}
    virtual Base2* clone() const { return NULL; }
protected:
    float data_base2;
};

class Derived : public Base1, public Base2
{
public:
    Derived() {}
    virtual ~Derived() {}
    virtual Derived* clone() const { return NULL; }
protected:
    float data_derived;
};

void test2()
{
    Derived* p = new Derived();
    Base1* p1 = p;
    Base2* p2 = p;
    std::cout << p << "\t" << p1 << "\t" << p2 << std::endl;
    std::cout << sizeof(Base1) << "\t" << (void*)((char*)p1 + sizeof(Base1)) << std::endl;
}

int main()
{
    //test1();
    test2();
    return 0;
}
