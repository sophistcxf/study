#include <iostream>
#include <string.h>

void test1()
{
    // 每个 int 设置成了 0x01010101，即 16843009
    int* p = new int[10];
    std::cout << "memset 1:" << std::endl;
    memset(p, 1, sizeof(int) * 10);
    for (int i = 0; i < 10; ++i) {
        std::cout << p[i] << std::endl;
    }

    // 每个 int 设置成了 0xFFFFFFFF, 即 -1
    std::cout << "memset 255:" << std::endl;
    memset(p, 255, sizeof(int) * 10);
    for (int i = 0; i < 10; ++i) {
        std::cout << p[i] << std::endl;
    }

    std::cout << "memset 256:" << std::endl;
    memset(p, 256, sizeof(int) * 10);
    for (int i = 0; i < 10; ++i) {
        std::cout << p[i] << std::endl;
    }

    std::cout << "memset 257:" << std::endl;
    memset(p, 257, sizeof(int) * 10);
    for (int i = 0; i < 10; ++i) {
        std::cout << p[i] << std::endl;
    }

    std::cout << "memset 258:" << std::endl;
    memset(p, 258, sizeof(int) * 10);
    for (int i = 0; i < 10; ++i) {
        std::cout << p[i] << std::endl;
    }

    std::cout << "memset 0:" << std::endl;
    memset(p, 0, 10);
    for (int i = 0; i < 10; ++i) {
        std::cout << p[i] << std::endl;
    }
}

int main()
{
    test1();
    return 0;
}
