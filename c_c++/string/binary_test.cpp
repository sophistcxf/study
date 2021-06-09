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

void test2()
{
    std::ostringstream oss;
    oss << (char)0xff;
    std::string str = oss.str();
    std::cout << str.data() << "\t" << str.size() << std::endl;
}

int main()
{
    //test1();
    test2();
    return 0;
}
