/*************************************************************************
  > File Name: test_numeric_limits.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 2016年09月17日 星期六 12时38分16秒
 ************************************************************************/

#include <iostream>
#include <limits>
#include <time.h>
using namespace std;

template <typename C>
void output_range(std::string desc)
{
    cout << desc << "\t"
         << std::numeric_limits<C>::max() << "\t"
         << std::numeric_limits<C>::min() << "\t"
         << std::numeric_limits<C>::lowest() << "\t"
         << std::numeric_limits<C>::epsilon() << "\t"
         << std::numeric_limits<C>::denorm_min() << "\t"
         << std::numeric_limits<C>::round_error() << "\t"
         << std::endl;
}

void test1()
{
    output_range<char>("char");
    output_range<unsigned char>("unsigned char");
    output_range<short>("short");
    output_range<unsigned short>("unsigned short");
    output_range<int>("int");
    output_range<unsigned>("unsigned");
    output_range<long>("long");
    output_range<unsigned long>("unsigned long");
    output_range<long long>("long long");
    output_range<unsigned long long>("unsigned long long");
    output_range<float>("float");
    output_range<double>("double");
    output_range<long double>("long double");
}

// 测试max的性能
void test2()
{
	unsigned count = 10 * 100 * 100 * 100;
	std::cout << time(NULL) << std::endl;
	for (unsigned i = 0; i < count; ++i)
	{
		unsigned temp = 0xFFFFFFFF;
		std::cerr << temp << std::endl;
	}
	std::cout << time(NULL) << std::endl;
	for (unsigned i = 0; i < count; ++i)
	{
		unsigned temp = std::numeric_limits<unsigned>::max();
		std::cerr << temp << std::endl;
	}
	std::cout << time(NULL) << std::endl;
}

void infinity()
{
    float f = std::numeric_limits<float>::max();
    std::cout << f << std::endl;
    std::cout << f + 1e30 << std::endl;
    std::cout << f + 1e20 << std::endl;
    std::cout << 2 * f << std::endl;    // 输出 inf
    std::cout << 2 * (double)f << std::endl;    // 6.80565e+38
    std::cout << (f + 1e20 > f ? 1 : 0) << std::endl;   // false
    std::cout << (f + 1e30 > f ? 1 : 0) << std::endl;   // true
    float f2 = f + 1e30;
    std::cout << (f2 > f ? 1 : 0) << std::endl; // false
    std::cout << (f2 == f ? 1 : 0) << std::endl; // true
    // 逐渐累加 float，加到多少时变成 inf
    float step = 1e25;  // step 小于 1e25 会很慢
    float cum = step;
    while (true) {
        if (f + cum == std::numeric_limits<float>::infinity()) {
            std::cout << "f + " << cum << " is inf" << std::endl;
            break;
        }
        cum += step;
    }

    f2 = std::numeric_limits<float>::infinity();
    std::cout << f2 << std::endl;

    char* c = (char*)&f;
    for (size_t i = 0; i < 4; ++i) {
        std::cout << hex << (int)c[i] << std::endl;
    }
    std::cout << std::endl;
    float f3 = f + 1e20;
    c = (char*)&f3;
    for (size_t i = 0; i < 4; ++i) {
        std::cout << hex << (int)c[i] << std::endl;
    }
    float f4 = f + 1e30;
    c = (char*)&f4;
    for (size_t i = 0; i < 4; ++i) {
        std::cout << hex << (int)c[i] << std::endl;
    }
    std::cout << (f4 == f ? 1 : 0) << std::endl;
    std::cout << (f4 > f ? 1 : 0) << std::endl;
    float f5 = 2 * f;
    c = (char*)&f5;
    for (size_t i = 0; i < 4; ++i) {
        std::cout << hex << (int)c[i] << std::endl;
    }
}

int main()
{
	//test1();
	//test2();
    infinity();
}
