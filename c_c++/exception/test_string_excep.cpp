#include <iostream>

void foo() {
    throw "foo exception";
}

int main() {
    try {
        foo();
    }
    catch (const char* str) {
        std::cout << str << std::endl;
    }
    catch (std::string str) {
        std::cout << str << std::endl;
    }
    catch (...) {
        std::cout << "undefined exception" << std::endl;
    }
    return 0;
}
