#include <iostream>
#include <type_traits>

void test_is_trivially_copyable() {
    struct A { int i; };

    struct B {
        int i,j;
        B (const B& x) : i(x.i), j(1) {};  // copy ctor
    };

    class C { int i; char* c; };

    std::cout << std::boolalpha;
    std::cout << "is_trivially_copyable:" << std::endl;
    std::cout << "int: " << std::is_trivially_copyable<int>::value << std::endl;
    std::cout << "A: " << std::is_trivially_copyable<A>::value << std::endl;
    std::cout << "B: " << std::is_trivially_copyable<B>::value << std::endl;
    std::cout << "C: " << std::is_trivially_copyable<C>::value << std::endl;
}

int main() {
    test_is_trivially_copyable();
    return 0;
}