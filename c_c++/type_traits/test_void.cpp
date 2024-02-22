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
 
int main()
{
    std::cout << std::boolalpha << std::left;
    SHOW(is_iterable<std::vector<double>>);
    SHOW(is_iterable<std::map<int, double>>);
    SHOW(is_iterable<double>);
    SHOW(is_iterable<A>);
}