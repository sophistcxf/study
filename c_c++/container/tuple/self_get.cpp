#include <array>
#include <iostream>
#include <string>

// 基类模板（空元组）
template <typename... Types>
class MyTuple;

// 特化为空元组
template <>
class MyTuple<> {};

/**
 * 递归定义
 */
template <typename Head, typename... Tail>
class MyTuple<Head, Tail...> {
public:
    MyTuple(Head head, Tail... tail)
        : head_(head), tail_(tail...) {}

    Head head_;
    MyTuple<Tail...> tail_;
};

// 获取类型的辅助模板
template <std::size_t index, typename Tuple>
struct TypeGetter;

// 特化版本，当index为0时
template <typename Head, typename... Tail>
struct TypeGetter<0, MyTuple<Head, Tail...>> {
    using type = Head;
};

// 递归版本，通过减少index继续递归
template <std::size_t index, typename Head, typename... Tail>
struct TypeGetter<index, MyTuple<Head, Tail...>> {
    using type = typename TypeGetter<index - 1, MyTuple<Tail...>>::type;
};

// 获取值的辅助模板
template <std::size_t index, typename Tuple>
struct TupleGetter;

// 特化版本，当index为0时
template <typename Head, typename... Tail>
struct TupleGetter<0, MyTuple<Head, Tail...>> {
    static Head& get(MyTuple<Head, Tail...>& tuple) {
        return tuple.head_;
    }
};

// 递归版本，通过减少index继续递归
template <std::size_t index, typename Head, typename... Tail>
struct TupleGetter<index, MyTuple<Head, Tail...>> {
    static typename TypeGetter<index, MyTuple<Head, Tail...>>::type& get(MyTuple<Head, Tail...>& tuple) {
        return TupleGetter<index - 1, MyTuple<Tail...>>::get(tuple.tail_);
    }
};

// 用户接口函数
template <std::size_t index, typename... Types>
typename TypeGetter<index, MyTuple<Types...>>::type& my_get(MyTuple<Types...>& tuple) {
    return TupleGetter<index, MyTuple<Types...>>::get(tuple);
}

int main() {
    MyTuple<int, double, std::string> t(1, 2.5, "hello");
    std::cout << my_get<0>(t) << std::endl; // 输出: 1
    std::cout << my_get<1>(t) << std::endl; // 输出: 2.5
    std::cout << my_get<2>(t) << std::endl; // 输出: hello

    // 修改 tuple 中的元素
    my_get<0>(t) = 42;
    my_get<1>(t) = 3.14;
    my_get<2>(t) = "world";

    // 打印修改后的值
    std::cout << my_get<0>(t) << std::endl; // 输出: 42
    std::cout << my_get<1>(t) << std::endl; // 输出: 3.14
    std::cout << my_get<2>(t) << std::endl; // 输出: world

    return 0;
}
