#include <utility>
#include <iostream>

template<typename Func, typename... Args>
void wrapper(Func&& func, Args&&... args) {
    // 对参数列表 args 进行完美转发
    func(std::forward<Args>(args)...);
}

// 示例目标函数
void example(int& lvalue, int&& rvalue) {
    std::cout << lvalue << "," << rvalue << std::endl;
}

int main() {
    int l = 10;
    wrapper(example, l, 5); // 5 是右值，l 是左值
    return 0;
}