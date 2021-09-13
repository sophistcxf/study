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

/**
 * 如果 msg 未初始化，则会报错
 * valgrind --tool=memcheck --track-origins=yes
 */
int main(int argc, char* argv[])
{
	Message msg;
	//msg.type = 'a';
	//msg.value = 10.0;
	std::cout << msg.type;
    std::cout << ",";
    std::cout << msg.value;
    std::cout << endl;
	return 0;
}
