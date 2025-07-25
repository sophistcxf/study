#include <tuple>
#include <iostream>

class A {
public:
    void foo() { std::cout << "A" << std::endl; }
};

class B {
public:
    void foo() { std::cout << "B" << std::endl; }
};

int main() {
    std::tuple<int, float, A*, B*> t;
    std::get<0>(t) = 12;
    std::get<1>(t) = 0.3;
    std::get<2>(t) = new A();
    std::get<3>(t) = new B();

    auto& x1 = std::get<0>(t);
    auto& x2 = std::get<1>(t);
    auto& x3 = std::get<2>(t);
    auto& x4 = std::get<3>(t);
    std::cout << x1 << "," << x2 << std::endl;
    x3->foo();
    x4->foo();
    return 0;
}
