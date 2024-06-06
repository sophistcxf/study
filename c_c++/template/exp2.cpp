#include <iostream>

class A {
public:
    template <typename T>
    void foo(T t) {
        std::cout << t << std::endl;
    }
};

class B : public A {
public:
    using A::foo;
    template <typename T>
    void foo(T t) {
        std::cout << "b" << std::endl;
    }
private:
    int n = 10;
};

int main() {
    A* p = new B;
    p->foo(10);
    return 0;
}