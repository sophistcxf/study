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

/*!
 * 调用 factory 时，Arg 会被复制构造
 */
template<typename T, typename Arg> 
T* factory1(Arg arg)
{
    return new T(arg);
}

/*!
 * 调用 factory 时，Arg虽然不会被复制构造，但是不能用
 * 右值作为参数
 */
template<typename T, typename Arg> 
T* factory2(Arg& arg)
{
    return new T(arg);
}

template<typename T, typename Arg> 
T* factory3(const Arg& arg)
{
    return new T(arg);
}

void test1()
{
    People* p1 = factory1<People, std::string>("zhangsan");
    //People* p2 = factory2<People, std::string>("zhangsan");   // 不能编译，常量不能传给一个非常量引用
    std::string zhangsan = "zhangsan";
    People* p2 = factory2<People, std::string>(zhangsan);

    People* p3 = factory3<People, std::string>("zhangsan");

    // factory1 和 factory2 各有优劣，factory1 需要额外复制构造，
    // factory2 不能传递常量
    // factory3 都可以，但是如果参数比较多
}

int main()
{
    test1();
    return 0;
}
