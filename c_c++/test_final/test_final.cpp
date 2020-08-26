#include <iostream>

#ifdef TEST1
struct Base
{
    virtual void foo() final;
};

struct Derived : public Base
{
    void foo() {}   // error, 不能 override 一个 virtual final 函数
};
#endif

#ifdef TEST2
struct Base
{
    void foo() final;   // error, 只有 virtual 才能声明为 final
};

struct Derived : public Base
{
    void foo() {}
};
#endif
 

#ifdef TEST3
struct Base
{
    virtual void foo();
};

struct Derived : public Base
{
    void foo() final;
};

struct A : public Derived
{
    void foo();     // error，不能 override 一个 virtual final 函数
};
#endif

#ifdef TEST4
struct Base
{
    virtual void foo() {};
} final;

struct Derived : public Base
{
    void foo() {};
};
#endif

int main()
{
    return 0;
}
