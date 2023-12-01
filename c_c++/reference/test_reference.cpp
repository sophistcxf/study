#include <iostream>

class Integer
{
public:
    Integer(int i) : value(i) {
        std::cout << "I'm Integer::Integer()" << std::endl;
    }
    int value = 0;
};

void test1() {
    int n = 10;
    int& lr = n;
    
    // 不能用一个左值引用向一个右值引用赋值
    // int&& rr1 = lr;

    int&& rr = std::move(n);

    int&& rr2 = 10;
    int& lr3 = rr2;
    lr3 = 20;
    std::cout << lr3 << "\t" << rr2 << std::endl;
}

int main() {
    test1();
    return 0;
}
