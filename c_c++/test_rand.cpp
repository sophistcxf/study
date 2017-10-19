/*************************************************************************
  > File Name: test_rand.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Thu 17 Aug 2017 02:05:47 PM CST
 ************************************************************************/

#include <stdlib.h>
#include <iostream>
using namespace std;

void test1()
{
	srand(0);
	for (unsigned i = 0; i < 10; ++i)
	{
		std::cout << rand() << endl;
	}
}

int main()
{
	test1();
	return 0;
}
