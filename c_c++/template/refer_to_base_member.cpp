/*!
 * 父类是模板类，子类引用父类中的成员
 */
#include <iostream>
using namespace std;

template <typename T>
class Base
{
public:
    void fun1()
    {
        std::cout << "I'm Base<T>::fun1" << std::endl;
    }
    void fun2()
    {
        std::cout << "I'm Base<T>::fun2" << std::endl;
    }
};

/*!
 * 模板类特化时，可以与非特化的成员函数不同，为什么要这样设计？引入复杂性
 * 既然要修改，为什么又要使用这个模板?
 * 为什么不强制要求不能重新定义类？
 */
template <>
class Base<int>
{
public:
    void fun2()
    {
        std::cout << "I'm Base<int>::fun2" << std::endl;
    }
};

template <typename T>
class Derived : public Base<T>
{
public:
    void fun3()
    {
        std::cout << "I'm Derived<T>::fun3" << std::endl;
        /*!
         * 这个调用有问题，因为编译器并不知道Base<T>是否被特化了
         * 如果被特化，且没有fun1()函数，则编译会失败，如Base<int>
         * 所以编译器认为错误，除非明确告诉编译器Base<T>有fun1函数
         * 如 this->fun1()
         */
        //fun1();

        this->fun1();
    }
};

int main()
{
    Derived<int> d;
    /*!
     * 编译失败，因为Derived<int>继承自Base<int>，而Base<in>并没有fun1()
     */
    //d.fun3();
    d.fun2();

    Derived<double> d2;
    d2.fun2();
    d2.fun3();

    return 0;
}
