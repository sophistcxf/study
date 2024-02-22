#include <iostream>

class Integer
{
public:
    Integer(int i) : value(i) {
        std::cout << "I'm Integer::Integer()" << std::endl;
    }
    int value = 0;
};

template<typename T>
class TD;

template <typename T>
void g(T& p) {
    std::cout << "g(T&)" << std::endl;
}


template <typename T>
void g(T&& p) {
    std::cout << "g(T&&)" << std::endl;
}

void test2() {
    g(10);
    int n = 10;
    g(n);
    int&& rr = 10;
    g(rr);
    g(std::move(n));
}

template <typename T>
void foo(T&& param) {
    // std::cout << typeid(T).name() << std::endl; 这个输出的不对
    //TD<T> x;    // 使编译失败，可以看T的推导结果是什么
    g(param);
}

template <typename T>
void foo2(typename std::remove_reference<T>::type& r) {
    g(std::forward<T&&>(r));
}

void test3() {
    //foo(10);    // T推导为int，T&& 推导为 int&&
    int n = 10;
    //foo(n);     // T推导为int&，T&&推导为int&
    int& lr = n;
    //foo(lr);    // T推导为int&
    int&& rr = 10;
    foo(lr);
    foo(rr);      // T推导为int&
    foo2<int&>(lr);
    foo2<int>(rr);

}

int main() {
    test2();
    //test3();
    return 0;
}
