#include <iostream>

/**
 * darwin不支持alias
 */
#ifndef __APPLE__
void __f() {
    std::cout << "hello world" << std::endl;
}
void f() __attribute__((weak, alias("__f")));
#endif

void test1() {
#ifndef __APPLE__
    f();
#endif
}

void __attribute__((deprecated("test2 will be deprecated"))) test2() {
}

int main() {
    test2();
    return 0;
}
