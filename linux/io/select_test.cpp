/*************************************************************************
  > File Name: select_test.cpp
  > Author: cxxxxf
  > Mail: cxxxxf@gmail.com
  > Created Time: Sat 04 May 2019 09:45:26 AM CST
 ************************************************************************/

#include <iostream>
#include <iomanip>
using namespace std;

#include <sys/select.h>

extern int errno;

void print_fd_set(const fd_set& fds)
{
    for (int i = 0; i < sizeof(fds.fds_bits) / sizeof(fd_mask); ++i) {
        std::cout << std::hex << std::setfill('0') << std::setw(sizeof(fd_mask)*2) << fds.fds_bits[i];
    }
    std::cout << std::endl;
}


/*!
 * 测试 select 相关数据结构的底层实现
 */
void test1()
{
    fd_set fds;
    /*!
     * fds.fds_bits 是用位来表示每个描述的打开情况
     */
#ifdef __USE_XOPEN
    std::cout << "fds.fds_bits elements size: " << sizeof(fds.fds_bits) / sizeof(fd_mask) << std::endl;
    std::cout << "fds.fds_bits sizeof: " << sizeof(fds.fds_bits) << std::endl;
    std::cout << "fds.fds_bits bit size: " << sizeof(fds.fds_bits) * 8 << std::endl;
#else
    std::cout << "fds.__fds_bits elements sizeof: " << sizeof(fds.__fds_bits) / sizeof(fd_mask) << std::endl;
    std::cout << "fds.__fds_bits: " << sizeof(fds.__fds_bits) << std::endl;
    std::cout << "fds.__fds_bits: " << sizeof(fds.__fds_bits) * 8 << std::endl;
#endif
    std::cout << "__NFDBITS macro: " << __NFDBITS << std::endl;
    std::cout << "FD_SETSIZE macro: " << FD_SETSIZE << std::endl;

#ifdef __USE_XOPEN
    FD_ZERO(&fds);
    print_fd_set(fds);
    FD_SET(10, &fds);
    print_fd_set(fds);
    /* 1024 实际上已经越界了 */
    FD_ZERO(&fds);
    FD_SET(1024, &fds);
    print_fd_set(fds);
#else
#endif
}

/*!
 * 简单测试 select，一直阻塞，直到标准输入有内容读取
 */
void test2()
{
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    int maxfd = 1;
    while (true) {
        int ret = select(maxfd, &fds, NULL, NULL, NULL);
        if (ret < 0) {
            std::cerr << "select error" << std::endl;
            continue;
        }
        else if (ret == 0) {
        }
        else {
            for (int i = 0; i < maxfd; ++i) {
                if (FD_ISSET(i, &fds)) {
                    char buf[1024] = {0};
                    read(i, buf, sizeof(buf)-1);
                    std::cout << buf << std::endl;
                }
            }
        }
    }
}

int main()
{
    //test1();
    test2();
    return 0;
}
