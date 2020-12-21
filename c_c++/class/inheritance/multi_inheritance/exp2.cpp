/**
 */

#include <iostream>

class A
{
public:
    virtual void only_in_a_virtual() {}
    void only_in_b_virtual() {}
public:
    int data_a;
};

class Base1 : public virtual A
{
public:
    Base1() {}
    virtual ~Base1() {}
    virtual void speakClearly() {}
    virtual Base1* clone() const { return NULL; }
    virtual void only_in_base1_virtual() {}
    void only_in_base1_nonvirtual() {}
protected:
    float data_base1;
};

class Base2 : public virtual A
{
public:
    Base2() {}
    virtual void mumble() {}
    virtual Base2* clone() const { return NULL; }
    virtual void only_in_base2_virtual() {}
    void only_in_base2_nonvirtual() {}
protected:
    float data_base2;
};

class Derived : public Base1, public Base2
{
public:
    Derived() {}
    virtual ~Derived() {}
    virtual Derived* clone() const { return NULL; }
    virtual void only_in_derived_virtual() {}
    void only_in_derived_nonvirtual() {}
protected:
    float data_derived;
};

int main()
{
    Derived* p = new Derived();
    Base1* p1 = p;
    Base2* p2 = p;
    std::cout << p << "\t" << p1 << "\t" << p2 << std::endl;
    std::cout << sizeof(Base1) << "\t" << (void*)((char*)p1 + sizeof(Base1)) << std::endl;
    return 0;
}
