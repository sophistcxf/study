#include <iostream>
#include <vector>
#include <utility>

/*!
 * std::move 的原型是（algorithm 也有一个 std::move）
 * C++11
 *  template< class T >
 *  typename std::remove_reference<T>::type&& move( T&& t ) noexcept;
 * C++14
 *  template< class T >
 *  constexpr std::remove_reference_t<T>&& move( T&& t ) noexcept;
 */
 
class Person
{
public:
    Person(std::string name) : name_(name), n_(0)
    {
        std::cout << "construct " << name_ << "\t" << n_ << std::endl;
    }
    ~Person()
    {
        std::cout << "destruct " << name_ << "\t" << n_ << std::endl;
    }
    Person(const Person& p)
    {
        name_ = p.name_;
        n_ = p.n_ + 1;
        std::cout << "construct copy from " << p.name_ << "_" << p.n_
                  << " to " << name_ << "_" << n_ << std::endl;
    }
    std::string name_;
    int n_;
private:
    //Person(const Person& p);
};

void test1()
{
    std::vector<Person> cvec;
    Person p("zhangsan");
    std::remove_reference<Person>::type&& r = std::move(p);
    cvec.push_back(r);
}

int main()
{
    test1();
    return 0;
}
