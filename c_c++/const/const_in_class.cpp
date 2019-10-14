#include <iostream>

class C1
{
public:
    C1() : n(10), pn(NULL) {}
    void fun() const
    {
        // n = 10;  // 编不过 assignment of member ‘C1::n’ in read-only object
        *pn = 10;
    }

    void fun2() {}

    int n;
    int* pn;
};

void test1()
{
    C1 c;
    c.n = 10;
    c.pn = new int;
    *c.pn = 10;
    c.fun();
}

void test2()
{
    //const C1 c; // 编不过 passing ‘const C1’ as ‘this’ argument of ‘void C1::fun2()’ discards qualifiers [-fpermissive]
    //c.fun2();
}

int main()
{
    test2();
    return 0;
}
