#include <cstdlib>
#include <iostream>

void test1() {
    const char* str = "12a";
    char** end = nullptr;
    long l = std::strtol(str, end, 10);
    std::cout << l << std::endl;

    str = "012a";
    l = std::strtol(str, end, 10);
    std::cout << l << std::endl;    // 输出12，即指定了base时，不再看prefix
                                    
    l = std::strtol(str, end, 0);
    std::cout << l << std::endl;    // 输出10，即按8进制输出，说明当不指定base时，看prefix

    str = "12a";
    l = std::strtol(str, end, 8);
    std::cout << l << std::endl;    // 即使prefix不是0，也是按8进制输出
                                    
    
    str = "12a*";
    l = std::strtol(str, end, 16);
    std::cout << l << std::endl;    // 即使prefix不是0x，也是按16进制输出
    

}

int main() {
    test1();
    return 0;
}
