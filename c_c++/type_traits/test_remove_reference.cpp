#include <iostream>
#include <type_traits>

template <class _Tp> struct my_remove_reference        {typedef _Tp type;};

void test2() {
    std::cout << std::boolalpha;
    std::cout << std::is_same<int, my_remove_reference<int>::type>::value << "\n";
    std::cout << std::is_same<int, my_remove_reference<int&>::type>::value << "\n";
    std::cout << std::is_same<int&, my_remove_reference<int&>::type>::value << "\n";
    std::cout << std::is_same<int&&, my_remove_reference<int&>::type>::value << "\n";
    std::cout << std::is_same<int, my_remove_reference<int&&>::type>::value << "\n";
    std::cout << std::is_same<int&, my_remove_reference<int&&>::type>::value << "\n";
    std::cout << std::is_same<int&&, my_remove_reference<int&&>::type>::value << "\n";
}

void test1() {
    std::cout << std::boolalpha;
 
    std::cout << "std::remove_reference<int>::type is int? "
              << std::is_same<int, std::remove_reference<int>::type>::value << '\n';
    std::cout << "std::remove_reference<int&>::type is int? "
              << std::is_same<int, std::remove_reference<int&>::type>::value << '\n';
    std::cout << "std::remove_reference<int&&>::type is int? "
              << std::is_same<int, std::remove_reference<int&&>::type>::value << '\n';
    std::cout << "std::remove_reference<const int&>::type is const int? "
              << std::is_same<const int, std::remove_reference<const int&>::type>::value << '\n';
    std::cout << "std::remove_reference<int&>::type is int&? "
              << std::is_same<int&, std::remove_reference<int&>::type>::value << '\n';
}

int main() {
    test2();
}