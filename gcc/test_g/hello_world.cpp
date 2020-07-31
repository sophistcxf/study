#include <iostream>

void foo(int i)
{
    std::cout << "hello world" << std::endl;
    std::cout << i << std::endl;
}

int main()
{
    foo(0);
    return 0;
}
