#include <iostream>

class C {
public:
    template <typename T>
    using pointer = typename T::pointer;
};

class B {
public:
    typedef int* pointer;
};

int main() {
    C::pointer<B> c;
    int n = 10;
    c = &n;
    std::cout << *c << std::endl;
    return 0;
}