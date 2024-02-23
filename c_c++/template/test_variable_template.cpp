#include <iostream>

template <typename T>
constexpr T pi{3};

template <typename T>
T pi2{3};

void test1() {
    std::cout << pi<double> << std::endl;
    std::cout << pi<int> << std::endl;

    std::cout << pi2<double> << std::endl;
    pi2<double> = 10;
    // pi2<double> 被修改了，说明 pi2<double> 只有一个实例对象
    std::cout << pi2<double> << std::endl;
}

int main() {
    test1();
}