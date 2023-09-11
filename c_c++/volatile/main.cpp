#include <iostream>

int foo(int n) {
    volatile int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += i;
    }
    return sum;
}

int foo2(int n) {
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += i;
    }
    return sum;
}

int main() {
    int n;
    std::cin >> n;
    std::cout << foo(n) << std::endl;
    std::cout << foo2(n) << std::endl;
    return 0;
}
