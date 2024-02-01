#include <iostream>

template <typename T>
void foo(T t) {}

template <typename T>
void foo1(T& t) {}

template <typename T>
void foo2(T&& t) {}


void test1() {
    int n = 10;
    int& lr = n;
    int&& rr = 10;
    int* p = &n;

    foo(10);
    foo(n);
    foo(lr);
    foo(rr);
    foo(p);

    //foo1(10);
    foo1(n);
    foo1(lr);
    foo1(rr);
    foo1(p);

    foo2(10);
    foo2(n);
    foo2(lr);
    foo2(rr);
    foo2(p);
}

int main() {
    return 0;
}