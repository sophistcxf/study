#include <iostream>
#include <algorithm>
#include <sys/time.h>

class A {
public:
    void foo() {
        std::cout << "A" << std::endl;
    }
};

void test1()
{
    int sum = 0;

    // 使用外部变量

    // 默认不能使用外部变量
    auto f = [](int x, int y) { return x + y; };
    f(10, 20);

    // cannot assign to a variable captured by copy in a non-mutable lambda
    // [=] 表示lambda外部值不可被修改，[&] 表示lambda外部值可以被修改
    // f = [=](int x, int y) { sum = x + y; return sum; }       // ERROR
    auto f1 = [&](int x, int y) { sum = x + y; return sum; };   // OK
    std::cout << f1(10 ,20) << std::endl;

    // cannot assign to a variable captured by copy in a non-mutable lambda
    // [=, &sum] 表示除了 sum，不可修改其他外部变量
    auto f2 = [=, &sum](int x, int y) { sum = x + y; return sum; };   // OK
    std::cout << f2(10 ,20) << std::endl;
    
    int product = 1;
    
    // cannot assign to a variable captured by copy in a non-mutable lambda
    // [&, product] 表示除了 product，可以修改其他外部变量
    auto f3 = [&, product](int x, int y) { sum = x + y; return sum; };   // OK
    std::cout << f3(10 ,20) << std::endl;

    A* a = new A();
    // 当传入的是指标，a本身是const，而非*a是const
    // 即相当于是 A* const a，而不是 const A*
    auto f4 = [a]() { a->foo(); };   // OK
    f4();
}

int sumArray(int* a, int size) {
    int sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += a[i];
    }
    return sum;
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

    int t = 0;

    gettimeofday(&tv_beg, NULL);
    t = 0;
    for (int i = 0; i < times; ++i) {
        int sum = 0;
        std::for_each(a, a+sizeof(a)/sizeof(int), [&] (int x) { sum += x; }); // OK
        t += sum;
    }
    gettimeofday(&tv_end, NULL);
    std::cout << (tv_end.tv_sec - tv_beg.tv_sec) * 1e6 + (tv_end.tv_usec - tv_beg.tv_usec) << std::endl;
    std::cout << t << std::endl;

    gettimeofday(&tv_beg, NULL);
    t = 0;
    for (int i = 0; i < times; ++i) {
        t += sumArray(a, sizeof(a)/sizeof(int));
    }
    gettimeofday(&tv_end, NULL);
    std::cout << (tv_end.tv_sec - tv_beg.tv_sec) * 1e6 + (tv_end.tv_usec - tv_beg.tv_usec) << std::endl;
    std::cout << t << std::endl;

    gettimeofday(&tv_beg, NULL);
    t = 0;
    for (int i = 0; i < times; ++i) {
        int sum = 0;
        for (int j = 0; j < sizeof(a)/sizeof(int); ++j) {
            sum += a[j];
        }
        t += sum;
    }
    gettimeofday(&tv_end, NULL);
    std::cout << (tv_end.tv_sec - tv_beg.tv_sec) * 1e6 + (tv_end.tv_usec - tv_beg.tv_usec) << std::endl;
    std::cout << t << std::endl;
}

int main()
{
    test1();
    //test2();
    return 0;
}
