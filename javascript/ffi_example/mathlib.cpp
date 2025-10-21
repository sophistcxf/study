#include "mathlib.h"
#include <cstring>
#include <cmath>
#include <algorithm>

// 加法函数
MATHLIB_API int add(int a, int b) {
    return a + b;
}

// 乘法函数
MATHLIB_API double multiply(double a, double b) {
    return a * b;
}

// 除法函数
MATHLIB_API float divide(float a, float b) {
    if (b == 0.0f) {
        return 0.0f; // 简单处理除以零的情况
    }
    return a / b;
}

// 字符串反转函数
MATHLIB_API const char* reverseString(const char* input) {
    if (input == nullptr) return nullptr;
    
    size_t len = strlen(input);
    char* reversed = new char[len + 1];
    strcpy(reversed, input);
    
    std::reverse(reversed, reversed + len);
    return reversed; // 调用者需要负责释放内存
}

// 创建点结构
MATHLIB_API Point createPoint(int x, int y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

// 计算两点距离
MATHLIB_API double distance(Point p1, Point p2) {
    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;
    return std::sqrt(dx*dx + dy*dy);
}
