#include <iostream>
#include <string>

struct T1
{
    int mem1;
    std::string mem2;
}; // implicit default constructor
 
struct T2
{
    int mem1;
    std::string mem2;
    T2(const T2&) {} // user-provided copy constructor
};                   // no default constructor
 
struct T3
{
    int mem1;
    std::string mem2;
    T3() {} // user-provided default constructor
};
 
std::string s{}; // class => default-initialization, the value is ""
 
int main()
{
    int n{};                // scalar => zero-initialization, the value is 0
    double f = double();    // scalar => zero-initialization, the value is 0.0
    int* a = new int[10](); // array => value-initialization of each element
                            //          the value of each element is 0
    
    // 使用隐式的默认构造函数
    // t1.mem1 是 zero-initialized，值为 0
    // t1.mem2 是 default-initialized，值为 ""
    T1 t1{};

    // 错误，T2 没有提供默认构造函数
    T2 t2{};

    // 用户自定义了默认构造函数
    // t3.mem1 是 default-initialized，是不确定的
    // t3.mem2 是 default-initialized，是""
    T3 t3{};

    T3 t3{10, "hello world"};
    std::cout << t3.mem1 << "\t" << t3.mem2 << "\n";

    delete[] a;
}