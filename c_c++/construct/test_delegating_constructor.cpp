#include <iostream>

class C {
public:
    int value1 = -1;
    int value2 = -1;

    C() {
        value1 = 1;
    }

    // 委托构造函数，c++11 引入
    C(int v) : C() {
        value2 = v;
    }
};

int main() {
    C c;
    std::cout << c.value1 << "\t" << c.value2 << std::endl;

    C c1(10);
    std::cout << c1.value1 << "\t" << c1.value2 << std::endl;

    return 0;
}
