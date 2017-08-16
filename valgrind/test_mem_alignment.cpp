/*************************************************************************
  > File Name: test_mem_alignment.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Fri 14 Jul 2017 02:42:54 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

struct Message
{
	char type;
	double value;
};

int main(int argc, char* argv[])
{
	Message msg;
	//msg.type = 'a';
	//msg.value = 10.0;
	std::cout << msg.type << "," << msg.value << endl;
	return 0;
}
