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
    
    Person(const Person&& p) {
        name_ = p.name_;
        n_ = p.n_ + 1;
        std::cout << "const rvalue construct copy from " << p.name_ << "_" << p.n_
                  << " to " << name_ << "_" << n_ << std::endl;
    }

    Person(Person&& p) {
        name_ = p.name_;
        n_ = p.n_ + 1;
        std::cout << "nonconst rvalue construct copy from " << p.name_ << "_" << p.n_
                  << " to " << name_ << "_" << n_ << std::endl;
    }
private:
    std::string name_;
    int n_;
};

void test1()
{
    std::vector<Person> cvec;
    cvec.reserve(10);
    std::cout << "1" << std::endl;
    Person p("zhangsan");
    std::cout << "2" << std::endl;
    std::remove_reference<Person>::type&& r = std::move(p);
    Person&& rp = std::move(p);
    std::cout << "3" << std::endl;
    cvec.push_back(std::move(p));

    const Person lisi("lisi");

    {
        //编译错误，由于lisi是const的，所以转出来是const Person&&
        //Person&& rp2 = std::move(lisi);
    }
    const Person&& rp3 = std::move(lisi);
    cvec.push_back(std::move(lisi));
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
    std::cout << v.size() << "," << v2.size() << std::endl;


    std::vector<int> v3 = v2;
    std::vector<std::vector<int> > v4;
    v4.push_back(std::move(v3));
    std::cout << v3.size() << "," << v4[0].size() << std::endl;


    std::string str1 = "zhangsan";
    std::string str2 = std::move(str1);
    std::cout << str1 << "\t" << str2 << std::endl;
}

int main()
{
    test1();
    //test2();
    //test3();
    return 0;
}
