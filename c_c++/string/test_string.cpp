// 使用 std::string 设置二进制数

#include <iostream>
#include <string>
#include <sstream>

void test1()
{
    std::string b;
    b.push_back((char)0xff);
    std::cout << b << "\t" << b.size() << std::endl;

    std::string hello = "hello";
    std::cout << hello << "\t" << hello.size() << std::endl;
    std::cout << *hello.begin() << "\t" << *(hello.end()-1) << "\t" << *hello.end() << std::endl;

    char sz_hello[] = "hello";
    std::cout << strlen(sz_hello) << "\t" << sizeof(sz_hello) << std::endl;
}

void test_binary()
{
    std::ostringstream oss;
    oss << (char)0xff;
    std::string str = oss.str();
    std::cout << str.data() << "\t" << str.size() << std::endl;
}

void test_trailing_zero() {
    const char sz[] = "hello world";
    std::string str1 = sz;
    std::string str2;
    str2.assign(sz);
    std::string str3;
    str3.assign(sz, strlen(sz));
    std::cout << strlen(sz) << std::endl;
    std::cout << sizeof(sz) << std::endl;
    std::cout << "str1 " << str1.size() << "\n";
    std::cout << "str2 " << str2.size() << "\n";
    std::cout << "str3 " << str3.size() << "\n";

    std::string str4;
    str4.assign(sz, sizeof(sz));
    std::cout << "str4 " << str4.size() << "\n";

    str4.push_back('c');
    std::cout << "str4 " << str4 << "\n";   // 无法输出'c'
}

int main()
{
    //test1();
    //test_binary();
    test_trailing_zero();
    return 0;
}
