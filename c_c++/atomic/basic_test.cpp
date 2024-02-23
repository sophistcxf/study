#include <atomic>
#include <iostream>

void test() {
    std::atomic<int> n;
    n = 10;
    n.fetch_add(1);
    std::cout << n << std::endl;

    //std::atomic<std::string> s; 编译失败，atomic只能用在trivially copyable type上
}

int main(int argc, char* argv[]) {
    test();
    return 0;
}