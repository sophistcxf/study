#include <iostream>
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
        std::cout << "People " << name_ << " copy lvalue construct!" << std::endl;
    }
    People(const People&& p)
    {
        name_ = p.name_;
        std::cout << "People " << name_ << " copy rvalue construct!" << std::endl;
    }
    ~People()
    {
        std::cout << "People " << name_ << " destruct!" << std::endl;
    }
    People& operator = (const People& p)
    {
        name_ = p.name_;
        std::cout << "People " << name_ << " const copy assignment!" << std::endl;
        return *this;
    }
    People& operator = (People& p)
    {
        name_ = p.name_;
        std::cout << "People " << name_ << " non-const copy assignment!" << std::endl;
        return *this;
    }
public:
    std::string name_;
};

People construct_people()
{
    People p("zhangsan");
    return p;
}

People construct_people2()
{
    People p("zhangsan");
    return std::move(p);
}

int main()
{
    std::cout << "1..." << std::endl;
    People p = construct_people();  // 不调用构造函数，编译器优化的

    std::cout << "2..." << std::endl;
    People p2 = construct_people2();    // 先调一次 copy rvalue construct，再调一次 destruct

    std::cout << "end..." << std::endl;
    return 0;
}
