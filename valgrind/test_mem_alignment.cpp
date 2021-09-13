/*************************************************************************
  > File Name: test_mem_alignment.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Fri 14 Jul 2017 02:42:54 PM CST
 ************************************************************************/

#include <iostream>
#include <string.h>
using namespace std;

/**
 * Message是16字节，type后面会被padding
 */
struct Message
{
	char type;
	double value;
};

void test1(Message msg) {
    std::cout << msg.type << std::endl;
}

/**
 * valgrind --tool=memcheck --track-origins=yes
 */
int main(int argc, char* argv[])
{
    std::cout << sizeof(Message) << std::endl;
	Message* msg = new Message();
    std::cout <<
	//msg->type = 'a';
	//msg->value = 10.0;

    Message* msg1 = new Message();
    std::cout << msg1->value << std::endl;
    memcpy(msg1, msg, sizeof(Message));
    test1(*msg1);

    delete msg;
    delete msg1;

	return 0;
}
