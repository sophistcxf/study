#ifndef MATHLIB_H
#define MATHLIB_H

#ifdef _WIN32
    #ifdef MATHLIB_EXPORTS
        #define MATHLIB_API __declspec(dllexport)
    #else
        #define MATHLIB_API __declspec(dllimport)
    #endif
#else
    #define MATHLIB_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

// 基本数学函数
MATHLIB_API int add(int a, int b);
MATHLIB_API double multiply(double a, double b);
MATHLIB_API float divide(float a, float b);

// 字符串处理函数
MATHLIB_API const char* reverseString(const char* input);

// 结构体相关函数
typedef struct {
    int x;
    int y;
} Point;

MATHLIB_API Point createPoint(int x, int y);
MATHLIB_API double distance(Point p1, Point p2);

#ifdef __cplusplus
}
#endif

#endif // MATHLIB_H
