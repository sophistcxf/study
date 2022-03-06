#include <iostream>

void test1() {
    char buf[128] = {'\0'};
    std::cin >> buf;
    /*
    for (int i = 0; i < 10; ++i) {
        std::cout << buf[i] << std::endl;
    }*/
    std::cout << buf;
    //std::cout << buf << std::endl;
}

int main() {
    test1();
    return 0;
}
