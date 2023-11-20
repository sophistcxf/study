#include <iostream>

void __attribute__((visibility ("default"))) foo() {
    std::cout << "I'm foo, attribute default" << std::endl;
}

void __attribute__((visibility ("hidden"))) foo1() {
    std::cout << "I'm foo1, attribute hidden" << std::endl;
}

void __attribute__((visibility ("internal"))) foo2() {
    std::cout << "I'm foo2, attribute internal" << std::endl;
}
