#include <iostream>

extern int sum(int, int);

int main(int argc, char* argv[]) {
    std::cout << "hello world" << std::endl;
    std::cout << sum(3, 4) << std::endl;
    return 0;
}
