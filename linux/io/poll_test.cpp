/*************************************************************************
  > File Name: poll_test.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Mon 29 Apr 2019 06:00:45 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include <poll.h>
#include <unistd.h>

void test1()
{
    struct pollfd poll_fd;
    /*!
     * 从标准输入中读
     */
    poll_fd.fd = 0;
    poll_fd.events = POLLIN;

    while (true) {
        int ret = poll(&poll_fd, 1, 2000);
        if (ret < 0) {
            std::cerr << "poll error" << std::endl;
            continue;
        }
        if (ret == 0) {
            std::cerr << "poll timeout" << std::endl;
            continue;
        }
        if (poll_fd.revents == POLLIN) {
            char buf[1024];
            read(0, buf, sizeof(buf)-1);
            std::cout << buf << std::endl;
        }
    }
}

int main()
{
    test1();
    return 0;
}
