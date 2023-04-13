// factorial as an integral_constant
#include <iostream>
#include <type_traits>
#include <vector>

template <unsigned n>
struct factorial : std::integral_constant<int,n * factorial<n-1>::value> {};

template <>
struct factorial<0> : std::integral_constant<int,1> {};

void test1() {
  std::cout << factorial<5>::value;  // constexpr (no calculations on runtime)
}

template <class T, T v>
struct my_integral_constant {
    static constexpr T value = v;
    typedef T value_type;
    typedef my_integral_constant<T, v> type;
    constexpr operator T() { return v; }
};

void test2() {
    my_integral_constant<int, 10> c;
    std::cout << c.operator int() << std::endl;
}

// foo_impl有两种重载，参数2表示是否为数值
template <typename T>
void foo_impl(T value, std::true_type) {
    // 对数值的实现
    std::cout << "数值" << "," << value << std::endl;
}

template <typename T>
void foo_impl(T value, std::false_type) {
    // 对非数值的实现
    std::cout << "非数值" << "," << sizeof(value) << std::endl;
}

template <typename T>
void foo(T value) {
    return foo_impl(value, std::is_arithmetic<T>{});
}

void test3() {
    // 在编译期可以确定调用哪个函数
    foo(10);
    foo("hello");
    foo(std::vector<int>());
}

int main() {
    //test2();
    test3();
    return 0;
}