#include <iostream>
#include <tuple>

// 类型列表
template <typename... Types>
struct TypeList {
    static constexpr std::size_t size = sizeof...(Types);
};

// 待实现的类型计数
template <typename T, typename TypeList>
struct TypeCounter;

// 基本情况
template <typename T>
struct TypeCounter<T, TypeList<>> {
    static constexpr std::size_t value = 0;
};

// 递归情况
template <typename T, typename... Types>
struct TypeCounter<T, TypeList<T, Types...>> {
    static constexpr std::size_t value = 1 + TypeCounter<T, TypeList<Types...>>::value;
};

// 不同类型情况
template <typename T, typename U, typename... Types>
struct TypeCounter<T, TypeList<U, Types...>> {
    static constexpr std::size_t value = TypeCounter<T, TypeList<Types...>>::value;
};

// 示例类型列表
using MyTypes = TypeList<int, float, double, char>;

// 示例数组
std::array<int, MyTypes::size> myArray;

class A {
public:
    template <typename T>
    void foo(T& a) {
        // 在这里你可以做一些工作，但是类型 T 必须是预先在 MyTypes 中声明的
    }
};

int main() {
    std::cout << MyTypes::size << std::endl;
    return 0;
}