#include <functional>
#include <iostream>

using namespace std::placeholders;

int fn(int a, int b) {
    return a - b;
}

int main() {
    auto bound_fn = std::bind (fn,100,_1);
    std::cout << bound_fn(5) << std::endl;

    auto fn2 = std::bind(fn, _2, _1);
    // 输出-95，而不是95
    std::cout << fn2(100, 5) << std::endl;
    return 0;
}
