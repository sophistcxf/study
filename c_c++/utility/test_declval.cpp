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

struct C {
    int foo() { 
        std::cout << "C::foo()" << std::endl;
        return 10;
    }

    int foo2(int n) {
        std::cout << "C::foo2()" << std::endl;
        return 10;
    }
};

struct D {
    double foo() {
        std::cout << "D::foo()" << std::endl;
        return 10.0f;
    }

    double foo2(int n) {
        std::cout << "D::foo2()" << std::endl;
        return 10.0f;
    }
};

template <typename T>
void test2() {
    decltype(std::declval<T>().foo()) n;
    decltype(std::declval<T>().foo2(10)) n2;
}

int main() {
    //test1();

    test2<C>();
    test2<D>();
}