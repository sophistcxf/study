#include <iostream>
#include <string>

/**
 * @brief 测试重载 basic_string& assign( size_type count, CharT ch );
 */
void test1() {
    std::string str;
    str.assign(10, 'a');
    std::cout << str << std::endl;
}

/**
 * @brief 测试重载 basic_string& assign( const CharT* s, size_type count );
 */
void test2() {
    std::string str;
    const char* sz = "hello world";
    str.assign(sz, 7);
    std::cout << str << std::endl;

    // assign 是copy一份
    str[3] = 'p';
    std::cout << sz << std::endl;
    std::cout << str << std::endl;


    // sz中有null characters
    // assign会会复制null character，但operator = 不会
    sz = "hello\0world";
    std::cout << sz << std::endl;
    str.assign(sz, 7);
    std::cout << str << std::endl;
    str = sz;
    std::cout << str << std::endl;
}

int main() {
    test2();
}