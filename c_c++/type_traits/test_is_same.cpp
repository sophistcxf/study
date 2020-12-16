#include <iostream>
#include <type_traits>
#include <cstdint>
 
void print_separator()
{
    std::cout << "-----\n";
}
 
void test1()
{
    std::cout << std::boolalpha;
 
    // some implementation-defined facts
    std::cout << std::is_same<int, std::int32_t>::value << '\n';
    // usually true if 'int' is 32 bit
    std::cout << std::is_same<int, std::int64_t>::value << '\n';
    // possibly true if ILP64 data model is used
 
    print_separator();
 
    // 'float' is never an integral type
    std::cout << std::is_same<float, std::int32_t>::value << '\n'; // false
 
    print_separator();
 
    // 'int' is implicitly 'signed'
    std::cout << std::is_same<int, int>::value << "\n";          // true
    std::cout << std::is_same<int, unsigned int>::value << "\n"; // false
    std::cout << std::is_same<int, signed int>::value << "\n";   // true
 
    print_separator();
 
    // unlike other types, 'char' is neither 'unsigned' nor 'signed'
    std::cout << std::is_same<char, char>::value << "\n";          // true
    std::cout << std::is_same<char, unsigned char>::value << "\n"; // false
    std::cout << std::is_same<char, signed char>::value << "\n";   // false
}

/*!
 * std::is_same_v 的 c++17 中引入
 */
void test2()
{
    std::cout << std::is_same_v<int, std::int32_t> << '\n';
}

int main()
{
    //test1();
    test2();
    return 0;
}
