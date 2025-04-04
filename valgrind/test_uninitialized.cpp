#include <iostream>
#include <stdlib.h>

class Message {
public:
    Message() {}
    char type;
    double value;
};

class Message_1 {
public:
    Message_1() { reset(); }
    void reset() {
        type = 'a';
        value = 0;
    }
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
 * 能检测出
 * 栈中的未初始化变量，--track-origins 只会显示在函数名上，见valgrind官方文档
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

void test5() {
    Message* msg = new Message();
    double d = msg->value;  // 这行不会报错，valgrind会持续跟踪未初始化的变量，在产生externally-visible behaviour时，才会报错
    std::cout << d << std::endl;
}

/**
 * 不会误报
 */
void test4() {
    Message_1* msg = new Message_1();
    std::cout << msg->value << std::endl;
}

/**
 * 也会报错
 */
void test6() {
    int n;
    if (n > 0) {
        std::cout << 10 << std::endl;
    }
}

/**
 * 这里判断 n>0时也会报错，即使输出的output在test7中并没有打印
 */
void test7_1(int cond, int& output) {
    if (cond > 0) {
        output = 10;
    }
}

void test7() {
    int n;
    int cond;
    test7_1(cond, n);
}

int main() {
    test7();
}
