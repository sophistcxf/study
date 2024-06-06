#include <iostream>
#include <typeinfo>

// TypeList 最初的定义
template<typename... Types>
struct TypeList {
    static constexpr std::size_t size = sizeof...(Types);
};

// 执行对特定类型的操作的函数模板
template<typename T>
void performAction() {
    // 举个例子，我们可以输出类型名
    std::cout << typeid(T).name() << std::endl;
}

// 遍历TypeList中的每个类型并执行操作的辅助函数模板
template <typename... Types>
void forEachType(TypeList<Types...>) {
    // 使用初始化列表展开技术（C++11特性）
    // 将 performAction<T> 的调用展开到对所有类型实例化
    (performAction<Types>(), ...);  // C++17的逗号折叠表达式
}

// 定义一个具体的TypeList
using MyTypes = TypeList<int, double, char>;

int main() {
    // 使用之前定义的 forEachType 函数模板处理所有类型
    forEachType(MyTypes{});
    return 0;
}