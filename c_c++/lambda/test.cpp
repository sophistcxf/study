#include <iostream>
#include <algorithm>
#include <sys/time.h>

void test1()
{
    int a[] = {1, 2, 3, 4, 5};
    std::for_each(a, a+sizeof(a)/sizeof(int), [=] (int x) { std::cout << x << std::endl;});

    int sum = 0;
    // cannot assign to a variable captured by copy in a non-mutable lambda
    // [=] 表示lambda外部值不可被修改，[&] 表示lambda外部值可以被修改
    // std::for_each(a, a+sizeof(a)/sizeof(int), [=] (int x) { total += x; });
    std::for_each(a, a+sizeof(a)/sizeof(int), [&] (int x) { sum += x; }); // OK
    std::cout << sum << std::endl;

    int product = 1;
    // cannot assign to a variable captured by copy in a non-mutable lambda
    // [=, &sum] 表示除了 sum，不可修改其他外部变量
    // std::for_each(a, a+sizeof(a)/sizeof(int), [=, &sum](int x) { sum += x; product *= x; });
    
    
    // cannot assign to a variable captured by copy in a non-mutable lambda
    // [=, sum] 表示除了 sum，可以修改其他外部变量
    // std::for_each(a, a+sizeof(a)/sizeof(int), [&, sum](int x) { sum += x; product *= x; });
}

void sumArray(int* a, int size) {
    int sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += a[i];
    }
}

/**
 * 测试 lambda 表示式的性能
 * 使用 lambda 性能低，耗时是 lambda > 使用函数 > 不使用函数
 */
void test2()
{
    int a[] = {1, 2, 3, 4, 5};

    int times = 1024 * 1024 * 100;

    struct timeval tv_beg;
    struct timeval tv_end;

    gettimeofday(&tv_beg, NULL);
    for (int i = 0; i < times; ++i) {
        int sum = 0;
        std::for_each(a, a+sizeof(a)/sizeof(int), [&] (int x) { sum += x; }); // OK
    }
    gettimeofday(&tv_end, NULL);
    std::cout << (tv_end.tv_sec - tv_beg.tv_sec) * 1e6 + (tv_end.tv_usec - tv_beg.tv_usec) << std::endl;

    gettimeofday(&tv_beg, NULL);
    for (int i = 0; i < times; ++i) {
        sumArray(a, sizeof(a)/sizeof(int));
    }
    gettimeofday(&tv_end, NULL);
    std::cout << (tv_end.tv_sec - tv_beg.tv_sec) * 1e6 + (tv_end.tv_usec - tv_beg.tv_usec) << std::endl;

    gettimeofday(&tv_beg, NULL);
    for (int i = 0; i < times; ++i) {
        int sum = 0;
        for (int j = 0; j < sizeof(a)/sizeof(int); ++j) {
            sum += a[j];
        }
    }
    gettimeofday(&tv_end, NULL);
    std::cout << (tv_end.tv_sec - tv_beg.tv_sec) * 1e6 + (tv_end.tv_usec - tv_beg.tv_usec) << std::endl;
}

int main()
{
    //test1();
    test2();
    return 0;
}
