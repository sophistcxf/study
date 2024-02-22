#include <utility>
#include <iostream>

template <class _Tp>
inline _Tp&&
my_forward(typename std::remove_reference<_Tp>::type& __t) {
    return static_cast<_Tp&&>(__t);
}

void test1() {
    int&& rr = 10;
    int& r = std::forward<int&>(rr);
}

void foo(int & n) {

}

void test2() {
    int&& rr = 10;
    // _Tp被推导为int&
    // _Tp&&就是int&&&，即int&，forward返回的是int&
    my_forward<int&>(rr);
    // _Tp被推导为int&&
    // _Tp&&就是int&& &&，即int&&，forward返回的是int&&
    my_forward<int&&>(rr);
    // _Tp被推导为int
    // _Tp&&就是int&&，forward返回的是int&&
    my_forward<int>(rr);

    int n = 10;
    my_forward<int>(n);
    my_forward<int&>(n);
    my_forward<int&&>(n);

    int lr = n;
    my_forward<int>(lr);
    my_forward<int&>(lr);
    my_forward<int&&>(lr);

}

int main() {
    test2();
    return 0;
}