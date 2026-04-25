#include <iostream>

struct Point {
    int x, y;
    constexpr Point(int a, int b) : x(a), y(b) {}
};

constexpr Point p(1, 2);   // OK，编译期对象

int main() {
    std::cout << p.x << "," << p.y << std::endl;
    return 0;
}
