#include <cstddef>
#include <type_traits>
#include <iostream>

class S;
void test1()
{
    int* p = NULL;
    int* p2 = static_cast<std::nullptr_t>(NULL);
    void(*f)(int) = NULL;
    int S::*mp = NULL;
    void(S::*mfp)(int) = NULL;
 
    // std::is_same_v 在 c++17 中
    if (std::is_same_v<decltype(NULL), std::nullptr_t>) {
        std::cout << "NULL implemented with type std::nullptr_t\n";
    } else {
        std::cout << "NULL implemented using an integral type\n";
    }
}

void test2()
{
    int* p = NULL;
    std::cout << *p << std::endl;
}

int main()
{
    test2();
    return 0;
}
