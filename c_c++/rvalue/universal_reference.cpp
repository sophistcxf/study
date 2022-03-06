#include <iostream>

template <typename T>
T add(T&& f1, T&& f2) {
    return f1 + f2;
}

template <typename T>
T add2(T& f1, T& f2) {
    return f1 + f2;
}

template <typename T1, typename T2>
double add3(T1&& f1, T2&& f2) {
    return f1 + f2;
}

template <typename T>
double add4(T&& f1, T&& f2) {
    return f1 + f2;
}
/**
 * v是个万能引用
 */
template <typename T>
void cout(T&& v) {
    std::cout << v << std::endl;
}

/**
 * 与cout相比，v是个右值引用，不能传左值
 */
template <typename T>
void cout1(const T&& v) {
    std::cout << v << std::endl;
}

template <typename T>
T cout2(const T&& v) {
    std::cout << v << std::endl;
    return v;
}

void test1() {
    std::cout << add(10, 20) << std::endl;
    //std::cout << add2(10, 20) << std::endl;   add2的参数是左值引用，不能传右值

    int f1 = 10, f2 = 20;
    std::cout << add2(f1, f2) << std::endl;
    //std::cout << add(f1, f2) << std::endl;  // error，说明add(T&& f1, T&& f2)中，f1和f2是右值引用，而非万能引用
    std::cout << add3(f1, f2) << std::endl;     // ok，《Effective Modern C++》中说，只有涉及到型别推导时，才是万能引用，否则是右值引用
    std::cout << add4(f1, f2) << std::endl;     // ok


    // 下面2个调用都可以，既可以传右值，也可以传左值
    cout(100);
    cout(f1);

    // 下面2个调用，cout1(100)可以，cout1(f1)不行
    cout1(100);
    //cout1(f1);
    
    cout2(100);
    //cout2(f1);  // error
}

int main() {
    test1();
    return 0;
}
