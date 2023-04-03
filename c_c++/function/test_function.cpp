/*************************************************************************
  > File Name: test_function.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com.com
  > Created Time: 2017年04月15日 星期六 12时42分06秒
 ************************************************************************/

#include <iostream>
#include <functional>
using namespace std;

int test2()
{
    //std::function<void(int)> f1;
    //void (*f1_target)(int) = f1.target<void(*)(int)>();
    //if (f1_target == NULL) { std::cout << "f1_target == NULL" << std::endl; }
}

void test_plus()
{
    std::plus<int> p;
    std::cout << p(10,15) << std::endl;
}

void test3(std::function<void(int)> f, int param) {
    f(param);
}

void foo(int n) {
    std::cout << n << std::endl;
}

int main(int argc, char* argv[])
{
    //test_plus();
    //test3(foo, 10);
    std::function<void(int)> f;
    std::cout << (f ? 1 : 0);
    return 0;
}
