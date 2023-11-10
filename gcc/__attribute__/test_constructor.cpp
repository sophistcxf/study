#include <iostream>

void __attribute__((constructor)) constructor() {
    std::cout << "program begin" << std::endl;
}

void __attribute__((destructor)) destructor() {
    std::cout << "program end" << std::endl;
}

int main() {
    return 0;
}
