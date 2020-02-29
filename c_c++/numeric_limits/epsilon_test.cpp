#include <iostream>
#include <limits>

int main()
{
    if (std::numeric_limits<int>::is_integer) {
        std::cout << std::numeric_limits<int>::epsilon() << std::endl;
    }
    else {
    }
    std::cout << std::numeric_limits<float>::epsilon() << std::endl;
    std::cout << std::numeric_limits<double>::epsilon() << std::endl;
    return 0;
}
