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

class B1 : public virtual A
{
public:
    B1() {}
    virtual ~B1() {}
    virtual void speakClearly() {}
    virtual B1* clone() const { return NULL; }
    virtual void only_in_B1_virtual() {}
    void only_in_B1_nonvirtual() {}
protected:
    float data_B1;
};

class B2 : public virtual A
{
public:
    virtual void only_in_B2_virtual() {}
    void only_in_B2_nonvirtual() {}
public:
    char data_B2;
};

class C1 : public virtual B1
{
public:
    virtual void only_in_C1_virtual() {}
    void only_in_C1_nonvirtual() {}
public:
    unsigned long long data_C1;
};

class D1 : public B2, public C1
{
public:
    virtual void only_in_d1_virtual() {}
    void only_in_d1_nonvirtual() {}
public:
    unsigned long long data_d1;
};

int main()
{
    return 0;
}
