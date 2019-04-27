/*************************************************************************
  > File Name: read_test.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: 六  4/27 19:44:03 2019
 ************************************************************************/

#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
using namespace std;

void test_nonblocking()
{
    char read_buf[128] = {};
    /* 将描述符 0 设置为非阻塞 */
    fcntl(0, F_SETFL, FNDELAY);
    read(0, read_buf, sizeof(read_buf));
    std::cout << "read nothing" << read_buf << std::endl;
    /* 将描述符 0 再次设置为阻塞 */
    fcntl(0, F_SETFL, 0);
    read(0, read_buf, sizeof(read_buf));
    std::cout << read_buf << std::endl;
}

void test_blocking()
{
    char read_buf[128] = {};
    read(0, read_buf, sizeof(read_buf));
    std::cout << read_buf << std::endl;
}

int main()
{
    test_nonblocking();
    //test_blocking();
    return 0;
}
