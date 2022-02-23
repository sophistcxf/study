#include <iostream>

class Integer
{
public:
    Integer(int i) : value(i) {
        std::cout << "I'm Integer::Integer()" << std::endl;
    }
    int value = 0;
};

void test1(const Integer& i) {
    printf("%x\n", &i);
    std::cout << i.value << std::endl;
}

void test2(const int& i) {
    printf("%x\n", &i);
    std::cout << i << std::endl;
}

int main() {
    std::cout << sizeof(Integer) << std::endl;
    test1(10);
    test1(10);
    test2(10);
    return 0;
}
