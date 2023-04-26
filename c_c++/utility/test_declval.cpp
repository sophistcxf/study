#include <utility>
#include <iostream>

struct A {
  virtual int value() = 0;
};

class B : public A {
    int val_;
public:
    B(int i,int j) : val_(i*j) {}
    int value() {
        return val_;
    }
};

template <typename T, typename = decltype(std::declval<T>() < std::declval<T>() )>
std::true_type  supports_less_than_test(const T&);

template<typename T>
struct has_less_than_operator : std::integral_constant<bool, std::is_same<std::true_type,
                                                                              decltype(supports_less_than_test(std::declval<T>()))>::value> {};

void test1() {
    //std::declval<A>();    // error，declval必须用在unevaluated operands中，如sizeof、decltype

    std::cout << sizeof(std::declval<A>()) << std::endl;
    decltype(std::declval<A>().value()) a;  // int a
    decltype(std::declval<B>().value()) b;  // int b
    decltype(B(0,0).value()) c;   // same as above (known constructor)
    a = b = B(10,2).value();
    std::cout << a << '\n';

    std::cout << has_less_than_operator<int>() << std::endl;
}

int main() {
    test1();
}