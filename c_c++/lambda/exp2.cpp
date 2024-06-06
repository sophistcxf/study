/**
 * 测试一下，lambda 传入对象时的构造情况
 * 
 */

#include <iostream>

class A{
public:
    A() { std::cout << "A" << std::endl; }
    A(const A& a) { std::cout << "A&" << std::endl; }
};

void foo(const A& a) {
    std::cout << "foo" << std::endl;
}

int main() {
    A a;    // 调用A()

    // 会调用 A(const A&) 再构造一份
    auto f = [a](){
        foo(a);
    };
    std::cout << "after f" << std::endl;
    f();
}