#include <iostream>

void foo(int& n) {
    std::cout << "foo(int& n)" << std::endl;
}

void foo(int&& n) {
    std::cout << "foo(int&& n)" << std::endl;
}

void g(int&& n) {
    foo(n);
}

void g2(int&& n) {
    foo(std::forward<int&&>(n));
}

int main() {
    int&& rr = 10;
    {
        // 下句编译失败
        // g(rr);
    }
    {
        // 成功编译并运行
        // 调用foo的版本是 foo(int& n)
        g(std::move(rr));
    }
    {
        // 成功编译并运行
        // 调用foo的版本是 foo(int&& n)
        g2(std::move(rr));
    }
    return 0;
}