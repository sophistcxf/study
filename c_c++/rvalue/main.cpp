/*************************************************************************
  > File Name: main.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Mon 27 Aug 2018 11:43:50 AM CST
 ************************************************************************/

#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

class People
{
public:
    People(std::string name) : name_(name)
    {
        std::cout << "People " << name_ << " construct!" << std::endl;
    }
    People(const People& p)
    {
        name_ = p.name_;
        std::cout << "People " << name_ << " copy construct!" << std::endl;
    }
    ~People()
    {
        std::cout << "People " << name_ << " destruct!" << std::endl;
    }
    People& operator = (const People& p)
    {
        name_ = p.name_;
        std::cout << "People " << name_ << " copy construct!" << std::endl;
    }
public:
    std::string name_;
};

/*!
 * foo() 返回的是右值
 */
People foo()
{
    People p("lisi");
    std::cout << &p << std::endl;
    return p;
}

void foo2(People& p)
{
    std::cout << "foo2 lvalue " << p.name_ << std::endl;
}

void foo2(People&& p)
{
    std::cout << "foo2 rvalue " << p.name_ << std::endl;
    // p 与foo 中的局部变量 lisi 指向的是同一块内存
    std::cout << &p << std::endl;
}

int main(int argc, char* argv[])
{
    People zhangsan("zhangsan");
    // zhangsan 是左值，所以调用 foo2(People& p)
    foo2(zhangsan);
    // foo() 是右值，所以调用 foo2(People&& p)
    foo2(foo());
    return 0;
}
