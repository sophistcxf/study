#include <iostream>
#include <type_traits>
 
class A {};
 
void test1() 
{
    std::cout << std::boolalpha;
    std::cout << std::is_pointer<A>::value << '\n';
    std::cout << std::is_pointer<A *>::value << '\n';
    std::cout << std::is_pointer<A &>::value << '\n';
    std::cout << std::is_pointer<int>::value << '\n';
    std::cout << std::is_pointer<int *>::value << '\n';
    std::cout << std::is_pointer<int **>::value << '\n';
    std::cout << std::is_pointer<int[10]>::value << '\n';
    std::cout << std::is_pointer<std::nullptr_t>::value << '\n';
}

template <typename T>
void checkIsPointer(T& t) {
    std::cout << std::boolalpha;
    std::cout << std::is_pointer<T>::value << "\n";
}

void test2() {
    A a;
    A* p = &a;
    checkIsPointer(a);  // false
    checkIsPointer(p);  // true
}

int main() {
    test2();
    return 0;
}
