/*!
 * 构造对象时，使用大括号
 * c++11 引入了使用大括号初始化对象的机制
 */

#include <initializer_list>
#include <string>
#include <iostream>

class Widget
{
public:
    Widget(int n, bool b)
    {
        std::cout << "Widget1" << std::endl;
    }
    Widget(int n, double d)
    {
        std::cout << "Widget2" << std::endl;
    }
    Widget(std::initializer_list<long double> l)
    {
        std::cout << "Widget initializer_list" << std::endl;
    }
    Widget(std::initializer_list<long double> l, int another)
    {
        std::cout << "Widget initializer list with another" << std::endl;
    }
};

void test2()
{
    Widget p1(10, true);
    Widget p2{10, true};    // 会调用 Widget(std::initializer_list<long double> l) 这个构造函数
    Widget p3(10, 10.0);
    //Widget p4(10, 10);  // ambiguous，编译失败
    Widget p5{10, true, 10, 10};

    // 大括号是可以嵌套的，最后一个元素类型是 int，就会调用 another 版本
    Widget p6{{10, true, 10}, 10};  // 调用 Widget(std::initializer_list<long double> l, int another)
    Widget p7{{10, true, 10, 10}};  // 调用 Widget(std::initializer_list<long double> l)
}

void test1()
{
    int x{0};
    int x1(0);
    auto x2(0);

    // deducing from brace-enclosed initializer list requires #include <initializer_list>
    auto x3{0};
}

int main()
{
    //test1();
    test2();
}
