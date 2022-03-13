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
        return *this;
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

// p是一个左值，但是具有右值引用的类型
void foo2(People&& p)
{
    std::cout << "foo2 rvalue " << p.name_ << std::endl;
    // p 与foo 中的局部变量 lisi 指向的是同一块内存
    std::cout << &p << std::endl;
}

void test1() {
    People zhangsan("zhangsan");
    // zhangsan 是左值，所以调用 foo2(People& p)
    foo2(zhangsan);
    // foo() 是右值，所以调用 foo2(People&& p)
    People&& rp = foo();
    foo2(foo());

    //People& p = People("zs"); // error，非const引用不能引用临时对象
    const People& p = People("zs"); // ok
    People&& p2 = People("zs"); // 右值引用，可以引用临时对象
}

void test2() {
    int&& n = 10;   // ok, n的类型是右值引用
    //int& n2 = 10; 错误
    std::cout << n << std::endl;
    n = 20;
    std::cout << n << std::endl;
    
    //int&& n2 = n;   // error, n是一个左值
    int&& n2 = std::move(n);    // ok, 将n强转为左值
    std::cout << n2 << std::endl;
    std::cout << &n << "," << &n2 << std::endl;
}

void test3() {
    std::vector<People> v;
    v.push_back(std::move(People("zs")));
}

int main(int argc, char* argv[])
{
    test1();
    //test2();
    //test3();
    return 0;
}
