#include <functional>
#include <iostream>

using namespace std::placeholders;

int fn(int a, int b) {
    return a - b;
}


void test1() {
    auto bound_fn = std::bind (fn,100,_1);
    std::cout << bound_fn(5) << std::endl;

    auto fn2 = std::bind(fn, _2, _1);
    // 输出-95，而不是95
    std::cout << fn2(100, 5) << std::endl;
}

template <class T>
void foo(T a) {
    std::cout << std::is_placeholder<T>::value << std::endl;
}

void test2() {
    foo(_1);    // 输出 1
    foo(_2);    // 输出 2
    foo(_10);   // 输出 10
    
    foo(10);    // 输出 0
}

int main() {
    test2();
    return 0;
}
