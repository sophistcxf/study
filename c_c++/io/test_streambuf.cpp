#include <fstream>
#include <iostream>

void test1() {
    char mybuffer [512] = {0};
    std::fstream filestr;
    filestr.rdbuf()->pubsetbuf(mybuffer,512);
    std::cout << filestr.is_open() << std::endl;

    filestr.open("test.txt");
    std::cout << filestr.is_open() << std::endl;

    // operations with file stream here.
    std::cout << filestr.gcount() << std::endl;
    std::cout << filestr.tellg() << std::endl;

    char sz[] = "hello world";
    filestr.write(sz, strlen(sz));
    std::cout << filestr.gcount() << std::endl;
    std::cout << filestr.tellg();
}

int main() {
    test1();
    return 0;
}
