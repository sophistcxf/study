#include <iostream>
#include <thread>
#include <unistd.h>
#include <string>

class A {
public:
    A() {
        std::cout << "A()" << std::endl;
    }
    ~A() {
        std::cout << "~A()" << std::endl;
        sleep(5);
    }
    std::string name = "A";
};

class B {
public:
    B() {
        std::cout << "B()" << std::endl;
    }
    ~B() {
        std::cout << "~B()" << std::endl;
        sleep(5);
        name = "invalid";
    }
    std::string name = "B";
};

A a;
B b;
int incre_num = 0;
void increnum(){
    while (true) {
        incre_num++;
        std::cout << incre_num << std::endl;
        std::cout << b.name << std::endl;
        sleep(1);
    }
}

int main() {
    std::thread th(increnum);
    std::cout << "before exit" << std::endl;
    int status = status;
    // 在exit中，将顺序析构b和a，如果线程th仍在运行，且访问了b和a，则有可能会崩溃
    exit(status);
    std::cout << "after exit" << std::endl;
}