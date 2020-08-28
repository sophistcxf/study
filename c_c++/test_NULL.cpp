#include <cstddef>
#include <type_traits>
#include <iostream>
class S;
int main()
{
    int* p = NULL;
    int* p2 = static_cast<std::nullptr_t>(NULL);
    void(*f)(int) = NULL;
    int S::*mp = NULL;
    void(S::*mfp)(int) = NULL;
 
    if (std::is_same_v<decltype(NULL), std::nullptr_t>) {
        std::cout << "NULL implemented with type std::nullptr_t\n";
    } else {
        std::cout << "NULL implemented using an integral type\n";
    }
}
