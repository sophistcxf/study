#include <time.h>
#include <iostream>
#include <sys/time.h>
#include <unistd.h>

extern int errno;

#define PRINT_TP(tp) \
do { \
    std::cout << tp.tv_sec << "," << tp.tv_nsec << std::endl; \
} \
while(false)

int output() {
    struct timespec tp;
    if (clock_gettime(CLOCK_REALTIME, &tp) != 0) {
        return -1;
    }
    std::cout << "CLOCK_REALTIME ";
    PRINT_TP(tp);

    struct timespec monotic_tp;
    if (clock_gettime(CLOCK_MONOTONIC, &monotic_tp) != 0) {
        return -1;
    }
    std::cout << "CLOCK_MONOTONIC ";
    PRINT_TP(monotic_tp);

    struct timespec monotic_raw_tp;
    if (clock_gettime(CLOCK_MONOTONIC_RAW, &monotic_raw_tp) != 0) {
        return -1;
    }
    std::cout << "CLOCK_MONOTONIC_RAW ";
    PRINT_TP(monotic_raw_tp);

#ifdef __linux__
    struct timespec boot_tp;
    if (clock_gettime(CLOCK_BOOTTIME, &boot_tp) != 0) {
        return -1;
    }
    PRINT_TP(boot_tp);
#endif

    return 0;
}

/**
 * @brief 使用adjtime修改时间，看一下对几个时间函数的影响
 */
void test_adjtime() {
    output();
    struct timeval old_tv;
    struct timeval delta;
    delta.tv_sec = 3600 * 26;
    delta.tv_usec = 0;
    if (adjtime(&delta, &old_tv) != 0) {
        std::cout << "adjtime failed, errno is " << errno << std::endl;
        return;
    }
    while (true) {
        output();
        sleep(1);
    }
}

/**
 * @brief  
 */
void test_clock_settime() {
    output();
    struct timespec tp;
    tp.tv_sec = 10000;
    tp.tv_nsec = 0;
    if (clock_settime(CLOCK_REALTIME, &tp) != 0) {
        std::cout << "clock_settime failed, errno is " << errno << std::endl;
        return;
    }
    output();
}

int main() {
    //test_adjtime();
    output();
    //test_clock_settime();
    return 0;
}