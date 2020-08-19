#include <iostream>

int main()
{
    const int i = 10;
    const int i1 = i;
    constexpr int j = 10;
    constexpr int j1 = j;   // 正确，因为编译期 j 的值已知
    int input;
    std::cin >> input;
    const int i2 = input;   // 正确
    constexpr int j2 = input;   // 无法编译，因为编译期 input 的值未知
    return 0;
}
