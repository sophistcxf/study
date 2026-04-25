#include <iostream>

class A {
public:
    static const int n2;
};

const int A::n2 = 10;

void test3() {
    std::cout << A::n2 << ::std::endl;

    const int* p = &(A::n2);

    int* p2 = (int*)p;
    *p2 = 20;
}

int main() {
    test3();
    return 0;
}
