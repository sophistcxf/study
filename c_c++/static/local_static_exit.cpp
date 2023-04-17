/**
 * 局部静态变量，在退出时的问题
 */
#include <iostream>
#include <thread>
#include <unistd.h>
#include <string>

class A {
public:
    A() {
        std::cout << "A()" << std::endl;
        name = new std::string("A");
    }
    ~A() {
        std::cout << "~A()" << std::endl;
        sleep(5);
        delete name;
        name = nullptr;
    }
    std::string* name = nullptr;
};

class B {
public:
    B() {
        std::cout << "B()" << std::endl;
        name = new std::string("B");
    }
    ~B() {
        std::cout << "~B()" << std::endl;
        sleep(5);
        delete name;
        name = nullptr;
    }
    std::string* name = nullptr;
};

A* getA() {
    static A a;
    return &a;
}

B* getB() {
    static B b;
    return &b;
}

int incre_num = 0;
void increnum(){
    while (true) {
        incre_num++;
        std::cout << incre_num << std::endl;
        A* p = getA();
        std::cout << *(p->name) << std::endl;
        B* p2 = getB();
        std::cout << *(p2->name) << std::endl;
        sleep(1);
    }
}

int main() {
    std::thread th(increnum);
    std::cout << "before exit" << std::endl;
    sleep(1);
    int status = status;
    // 在exit中，将顺序析构b和a，如果线程th仍在运行，且访问了b和a，则有可能会崩溃
    exit(status);
    std::cout << "after exit" << std::endl;
}