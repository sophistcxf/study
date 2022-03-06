#include <iostream>

static int g_uninited;
static int g_inited = 10;

static int add_static(int a, int b) {
    return a + b;
}

int add(int a, int b) {
    return a + b;
}

int main()
{
    std::cout << "hello world" << std::endl;
    std::cout << add(10, 20) << std::endl;
    std::cout << g_inited << "," << g_uninited << std::endl;
    return 0;
}
