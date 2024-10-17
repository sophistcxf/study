#include <iostream>

void foo2() {
    int sum = 0;
    for (int i = 0; i < 1024 * 1024 * 1024; ++i) {
        sum += i;
    }
    std::cout << sum << std::endl;
    return;
}

void foo1() {
    foo2();
    int sum = 0;
    for (int i = 0; i < 1024 * 1024; ++i) {
        sum += i;
    }
    std::cout << sum << std::endl;
    return;
}

int main() {
    foo1();
    return 0;
}
