#include <iostream>

class __attribute__((aligned(32))) A {
public:
    char arr1[30];
    char arr2[30];
    unsigned long long n;
};

int main() {
    std::cout << sizeof(A) << std::endl;
    A a;
    printf("%x\t%x\t%x\t%x\n", &a, &(a.arr1[0]),  &(a.arr2[0]), &(a.n));
}
