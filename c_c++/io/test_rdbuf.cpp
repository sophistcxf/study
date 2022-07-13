#include <iostream>

void test1() {
    std::streambuf* buf = std::cin.rdbuf();
}

int main() {
    test1();
    return 0;
}
