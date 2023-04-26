#include <iostream>
#include <string>

struct A {
public:
    std::string name = "zhangsan";
};

int main() {
    double f = 1.0;
    decltype(f) f2 = 20;
    std::cout << f2 << std::endl;
    decltype(f)& f3 = f;
    f3 = 30;
    std::cout << f << std::endl;

    const double lf = 1.0;
    decltype(lf) lf1 = lf;
    //lf1 = 10; // error，不能修改 const double
    
    int n = 10;
    int& rn = n;
    auto n1 = rn;           // n1 的类型是 int
    n1 = 20;
    std::cout << rn << std::endl;
    decltype(rn) n2 = rn;   // n2 的类型是 int&
    n2 = 30;
    std::cout << rn << std::endl;

    A a;
    decltype(auto) b = a;   // C++14的用法
    std::cout << b.name << std::endl;

    decltype(a) b1 = a;
    std::cout << b1.name << std::endl;

    //decltype(f) b3 = a;
    //std::cout << b3.name << std::endl;

    return 0;
}