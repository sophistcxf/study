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

int main()
{
	test1();
	//test2();
}
