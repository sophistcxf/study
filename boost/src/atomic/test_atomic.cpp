/*************************************************************************
  > File Name: test_atomic.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sat 17 Jun 2017 07:36:56 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include <boost/atomic.hpp>

int test1()
{
	boost::atomic<int> i(0);
	std::cout << i.fetch_add(1, boost::memory_order_release) << std::endl;
	std::cout << i.load() << std::endl;
	std::cout << i.fetch_add(20) << std::endl;
	std::cout << i.load() << std::endl;
	return 0;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "please input mode" << std::endl;
		return -1;
	}
	int mode = atoi(argv[1]);
	switch (mode)
	{
		case 1:
			test1();
			break;
		default:
			std::cerr << "invalid mode" << std::endl;
			return -1;
	}
}
