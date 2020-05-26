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
    }
    void fun2()
    {
    }
};

template <>
class Base<int>
{
public:
    void fun2()
    {
    }
};

template <typename T>
class Derived : public Base<T>
{
public:
    void fun3()
    {
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
     * 编译失败，因为Derived<int>继承自Base<int>
     */
    //d.fun3();
    return 0;
}