#include <type_traits>

int main()
{
    int i = 0;
    static_assert(i == 0);
    return 0;
}
