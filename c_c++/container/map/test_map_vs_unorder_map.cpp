#include <map>
#include <unordered_map>
#include <iostream>
#include <sys/time.h>

void fun1() {
    std::map<int, int> m;
    for (int i = 0; i < 10e6; ++i) {
        m[i] = 0;
    }
}

void fun2() {
    std::unordered_map<int, int> m;
        for (int i = 0; i < 10e6; ++i) {
        m[i] = 0;
    }
}

int main() {
    struct timeval t1;
    gettimeofday(&t1, NULL);

    fun1();

    struct timeval t2;
    gettimeofday(&t2, NULL);

    std::cout << t2.tv_sec * 10e6 + t2.tv_usec - t1.tv_sec * 10e6 - t1.tv_usec << std::endl;

    gettimeofday(&t1, NULL);

    fun2();

    gettimeofday(&t2, NULL);

    std::cout << t2.tv_sec * 10e6 + t2.tv_usec - t1.tv_sec * 10e6 - t1.tv_usec << std::endl;

    return 0;
}