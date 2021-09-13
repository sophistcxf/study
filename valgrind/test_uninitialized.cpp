#include <iostream>

struct Message {
    char type;
    double value;
};

void test1() {
    int n;
    std::cout << n << std::endl;
}

void test2() {
    Message msg;
    std::cout << msg.value << std::endl;
}

void test3() {
    Message* msg = new Message();
    std::cout << msg->value << std::endl;
}

int main() {
    test3();
}
