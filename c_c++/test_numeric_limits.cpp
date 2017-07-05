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

void test1()
{
  cout << std::numeric_limits<int>::max() << "\t" << std::numeric_limits<int>::min() << endl;
  cout << std::numeric_limits<unsigned>::max() << "\t" << std::numeric_limits<unsigned>::min() << endl;
  cout << std::numeric_limits<float>::max() << "\t" << std::numeric_limits<float>::min() << endl;
  cout << std::numeric_limits<double>::max() << "\t" << std::numeric_limits<double>::min() << endl;
  if (std::numeric_limits<float>::min() < 0)
  cout << "dd";
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
//	test1();
	test2();
}
