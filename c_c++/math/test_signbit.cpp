#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
using namespace std;

/*!
 * 测试 std::signbit
 */
void test_signbit()
{
    /* 判断给定的 floating point number 是否是负的 */
    std::cout << std::boolalpha << "signbit(+0.0) = " << std::signbit(+0.0) << std::endl
        << "signbit(-0.0) = " << std::signbit(-0.0) << std::endl;
    /* 
     * 扩展，如何判断 floating point number 是负的？此函数是怎么实现的？
     * CPP reference:
     * This function detects the sign bit of zeroes, infinities, and NaNs. Along with std::copysign, 
     * std::signbit is one of the only two portable ways to examine the sign of a NaN
     */
}

int main(int argc, char* argv[])
{
    test_signbit();
	return 0;
}
