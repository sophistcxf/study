#include <time.h>
#include <iostream>
#include <errno.h>
#include <string.h>

extern int errno;

void test1()
{
    int clk_id = 0;
    struct timespec res;
    int ret = clock_getres(clk_id, &res);
    if (ret != 0) {
        return;
    }
    std::cout << "res " << "\ttv_sec: " << res.tv_sec << "\tv_nsec: " << res.tv_nsec << std::endl;
    struct timespec tp;
    ret = clock_gettime(clk_id, &tp);
    std::cout << "tp" << "\ttv_sec: " << tp.tv_sec << "\ttv_nsec: " << tp.tv_nsec << std::endl;
    struct timespec tp1;
    ret = clock_gettime(clk_id, &tp1);
    std::cout << "tp1" << "\ttv_sec: " << tp1.tv_sec << "\ttv_nsec: " << tp1.tv_nsec << std::endl;

    // 如果获取一个不存在的 resolution，则返回错误，errno 设为 EINVAL
    int not_exist_id = 999;
    ret = clock_getres(not_exist_id, &res);
    if (ret != 0) {
        std::cout << "error " << errno << "\tstrerror: " << strerror(errno) << std::endl;
    }
}

int main(int argc, char* argv[])
{
    test1();
    return 0;
}
