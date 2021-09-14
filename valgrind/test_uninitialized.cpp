#include <iostream>
#include <stdlib.h>

class Message {
public:
    Message() {}
    char type;
    double value;
};

/**
 * 能检测出，但行号有偏差
 */
void test1() {
    int n;
    std::cout << n << std::endl;
}

/**
 * 能检测出，但行号有偏差
 */
void test2() {
    Message msg;
    std::cout << msg.value << std::endl;
}

/**
 * 能检测出
 */
void test3() {
    Message* msg = new Message();
    std::cout << msg->value << std::endl;
}

int main() {
    test2();
}
