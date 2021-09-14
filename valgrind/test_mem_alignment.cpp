/*************************************************************************
  > File Name: test_mem_alignment.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Fri 14 Jul 2017 02:42:54 PM CST
 ************************************************************************/

#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

/**
 * Message是16字节，type后面会被padding
 */
class Message
{
public:
    Message() : type('a'), value(0) {}
	char type;
	double value;
};

/**
 * 取消了padding，所以sizeof(Message_1)=9
 */
class Message_1
{
public:
    Message_1() : type('a'), value(0) {}
	char type;
	double value;
} __attribute__((packed));

#pragma pack(push,1)
class Message_2
{
public:
    Message_2() : type('a'), value(0) {}
	char type;
	double value;
};

#pragma pack(pop)

/**
 * 这里valgrind会报错，虽然Message构造函数已经初始化了成员变量，但是没有初始化
 * type和value之间padding的数据，所以也会报错
 */
void test1() {
    Message msg;
    FILE* pf = fopen("msg.dat", "wb");
    fwrite(&msg, sizeof(Message), 1, pf);
    fclose(pf);
}

/**
 * 先使用memset整体初始化后，padding的数据也会被初始化，则没有问题
 */
void test2() {
    Message msg;
    memset(&msg, 0, sizeof(Message));
    FILE* pf = fopen("msg.dat", "wb");
    fwrite(&msg, sizeof(Message), 1, pf);
    fclose(pf);
}

/**
 * valgrind不报错，Message_1取消了padding
 */
void test3() {
    std::cout << sizeof(Message_1) << std::endl;
    Message_1 msg;
    FILE* pf = fopen("msg.dat", "wb");
    fwrite(&msg, sizeof(msg), 1, pf);
    fclose(pf);
}

void test4() {
    std::cout << sizeof(Message_2) << std::endl;
    Message_1 msg;
    FILE* pf = fopen("msg.dat", "wb");
    fwrite(&msg, sizeof(msg), 1, pf);
    fclose(pf);
}
/**
 * valgrind --tool=memcheck --track-origins=yes
 */
int main(int argc, char* argv[])
{
    test4();
	return 0;
}
