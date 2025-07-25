#include <iostream>

template <typename... Types>
class MyTuple;

template <>
class MyTuple<> {};

template <typename Head, typename... Tail>
class MyTuple<Head, Tail...> {
public:
    MyTuple(Head head, Tail... tail) : head_(head), tail_(tail...) {}
    Head head_;
    MyTuple<Tail...> tail_;
};

int main() {
    MyTuple<> a;

    MyTuple<int, float> a2 = {11, 0.1f};
    return 0;
}
