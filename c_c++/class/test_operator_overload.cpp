#include <string>
#include <iostream>

class Person {
    public:
        // 重载bool()，可使对象作为bool类型使用
        operator bool() {
            return !name.empty();
        }

        std::string name;
};

int main() {
    Person zhangsan;
    std::cout << (zhangsan ? "initialized" : "not initialized") << std::endl;
    zhangsan.name = "zhangsan";
    std::cout << (zhangsan ? "initialized" : "not initialized") << std::endl;
}
