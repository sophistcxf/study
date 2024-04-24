#include <iostream>

template <typename G>
void g(G& p) {
    std::cout << "g(G&)" << std::endl;
}

template <typename G>
void g(G&& p) {
    std::cout << "g(G&&)" << std::endl;
}

template <typename T>
void foo(T&& param) {
    g(param);
}

void test3() {
	int n = 10;
	int& lr = n;
    int&& rr = 10;
    foo(lr);
    foo(rr);      // T推导为int&，所以T&&&为T&, 调用g的左值版本
    foo(std::forward<int&&>(rr));
}

int main() {
    test3();
}
