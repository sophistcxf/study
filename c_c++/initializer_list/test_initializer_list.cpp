#include <initializer_list>
#include <iostream>

int main()
{
    std::initializer_list<int> l = {1, 2, 3};
    for (const auto& p : l) {
        std::cout << p << std::endl;
    }
    return 0;
}
