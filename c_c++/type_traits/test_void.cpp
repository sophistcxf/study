#include <iomanip>
#include <iostream>
#include <map>
#include <type_traits>
#include <vector>
 
// Variable template that checks if a type has begin() and end() member functions
template<typename, typename = void>
constexpr bool is_iterable{};

template<typename T>
constexpr bool is_iterable<
    T,
    std::void_t<decltype(std::declval<T>().begin()),
                decltype(std::declval<T>().end())
    >
> = true;
 
class A {};
 
#define SHOW(...) std::cout << std::setw(34) << #__VA_ARGS__ \
                            << " == " << __VA_ARGS__ << '\n'

void test1() {
    std::cout << std::boolalpha << std::left;
    SHOW(is_iterable<std::vector<double>>);
    SHOW(is_iterable<std::map<int, double>>);
    SHOW(is_iterable<double>);
    SHOW(is_iterable<A>);
}
 

std::void_t<int> test2() {
    //return 0; // test2的返回值是void
}

std::void_t<int, double> test3() {
    //return 0; // test3的返回值为void
}


// primary template handles types that have no nested ::type member:
template<class, class = void>
struct has_type_member : std::false_type {};
 
// specialization recognizes types that do have a nested ::type member:
template<class T>
struct has_type_member<T, std::void_t<typename T::type>> : std::true_type {};

struct B {
    typedef int type;
    int specified_member;
};

template<class, class = void>
struct has_specified_member : std::false_type {};
 
template<class T>
struct has_specified_member<T, std::void_t<decltype(T::specified_member)>> : std::true_type {};

/**
 * @brief 
 *  判断模板类T是否拥有指定的内嵌类型 ::type
 *  判断模板类T是否拥有指定的成员变量
 */
void test4() {
    std::cout << std::boolalpha;
    std::cout << has_type_member<double>().value << std::endl;          // false
    std::cout << has_type_member<B>().value << std::endl;               // true
    std::cout << has_specified_member<double>().value << std::endl;     // false
    std::cout << has_specified_member<B>().value << std::endl;          // true
}

int main()
{
    test4();
}