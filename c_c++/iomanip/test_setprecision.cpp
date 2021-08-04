/*************************************************************************
  > File Name: test_iomanip.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 六  7/15 16:39:34 2017
 ************************************************************************/

#include <iostream>
#include <iomanip>

void test1()
{
    double f =3.141592654;
    std::cout << f << "\n";
    // 3.1416
    std::cout << std::setprecision(5) << f << '\n';
    // 3.14159
    std::cout << std::setprecision(10) << f << '\n';
    std::cout << std::fixed;
    // 3.14159
    std::cout << std::setprecision(5) << f << '\n';
    // 3.141590000
    std::cout << std::setprecision(10) << f << '\n';
}

int main()
{
    test1();
    return 0;
}
