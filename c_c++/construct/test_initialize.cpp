#include <iostream>
using namespace std;

class C1
{
public:
    C1(int n) : value(n) { std::cout << "C1 construct" << std::endl; }
    int value;
};

class C2
{
public:
    C2(int n) : value(n) { std::cout << "C2 construct" << std::endl; }
    int value;
};

class C
{
public:
    /* 初始化顺序是按声明顺序，即使初始化列表是按c2, c1 */
    C() : c2(0), c1(10) {}
    C1 c1;
    C2 c2;
};

int main()
{
    C c;
    return 0;
}
