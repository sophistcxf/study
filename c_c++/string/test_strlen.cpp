#include <string>
#include <iostream>

int main() {
    char sz[] = "hello world";
    std::cout << strlen(sz) << std::endl;

    std::string str = "hello world";
    std::cout << str.size() << std::endl;

    return 0;
}
