/*************************************************************************
  > File Name: test_function.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com.com
  > Created Time: 2017年04月15日 星期六 12时42分06秒
 ************************************************************************/

#include <iostream>
#include <functional>
using namespace std;

void test_plus()
{
    std::plus<int> p;
    std::cout << p(10,15) << std::endl;
}

int main(int argc, char* argv[])
{
    test_plus();
    return 0;
}
