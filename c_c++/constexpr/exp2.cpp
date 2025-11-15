#include <iostream>

class A {
public:
    static constexpr int n2 = 10;
    char arr[n2];
};

void test3() {
    std::cout << A::n2 << ::std::endl;

    // 下面这句会出现链接错误，找不到符号A::n2
    // const int* p = &(A::n2);
}

int main() {
    test3();
    return 0;
}
