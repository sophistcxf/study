#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <iomanip>

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
    /*
    Person(const Person&& p) {
        name_ = p.name_;
        n_ = p.n_ + 1;
        std::cout << "rvalue construct copy from " << p.name_ << "_" << p.n_
                  << " to " << name_ << "_" << n_ << std::endl;
    }
    */
    std::string name_;
    int n_;
private:
    //Person(const Person& p);
};

void test1()
{
    std::vector<Person> cvec;
    std::cout << "1" << std::endl;
    Person p("zhangsan");
    std::cout << "2" << std::endl;
    //std::remove_reference<Person>::type&& r = std::move(p);
    std::cout << "3" << std::endl;
    cvec.push_back(std::move(p));
}

void test2() {
    std::string str = "Salut";
    std::vector<std::string> v;

    // uses the push_back(const T&) overload, which means
    // we'll incur the cost of copying str
    v.push_back(str);
    std::cout << "After copy, str is " << str << '\n';

    // uses the rvalue reference push_back(T&&) overload,
    // which means no strings will be copied; instead, the contents
    // of str will be moved into the vector.  This is less
    // expensive, but also means str might now be empty.
    v.push_back(std::move(str));
    std::cout << "After move, str is " << str << '\n';

    std::cout << "The contents of the vector are { " << v[0]
        << ", " << v[1] << " }\n";
}

void test3() {
    std::vector<int> v;
    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
    }
    std::vector<int> v2 = std::move(v);
    // v.size() will be 0, v2.size() will be 10
    std::cout << v.size() << "," << v2.size() << std::endl;
}

int main()
{
    //test1();
    //test2();
    test3();
    return 0;
}
