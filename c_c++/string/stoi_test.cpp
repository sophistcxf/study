#include <iostream>
#include <string>

void test1()
{
    std::string s = "1024";
    std::cout << std::stoi(s) << std::endl;

    s = "1a024";
    std::cout << std::stoi(s) << std::endl;

    s = "abcd";
    try {
        std::cout << std::stoi(s) << std::endl;
    }
    catch (std::invalid_argument e) {
        std::cout << e.what() << std::endl;
    }

    s = "9999999999999999999999999999999";
    try {
        std::cout << std::stoi(s) << std::endl;
    }
    catch (std::out_of_range e) {
        std::cout << e.what() << std::endl;
    }
}

int main()
{
    test1();
    return 0;
}
