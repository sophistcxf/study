#include <iostream>
#include <array>

int main()
{
    std::array<int, 4> arr = {1, 2, 3, 4};
    for (auto i : arr) {
        std::cout << i << std::endl;
    }
}
