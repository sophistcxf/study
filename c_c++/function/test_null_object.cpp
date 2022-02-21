#include <iostream>

class C {
public:
    void foo() {
        std::cout << "hello world" << std::endl;
    }
};

class C1 {
public:
    virtual void foo() {
        std::cout << "hello world" << std::endl;
    }
};

void test1() {
    // 正常输出
    C* p = nullptr;
    p->foo();

    // segmentation fault
    C1* p1 = nullptr;
    p1->foo();
}

int main() {
    test1();
    return 0;
}
