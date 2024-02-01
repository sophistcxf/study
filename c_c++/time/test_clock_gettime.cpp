#include <time.h>
#include <iostream>

long toNanoSeconds(const timespec& tp) {
    return tp.tv_sec * 1e9 + tp.tv_nsec;
}

#define PRINT_TP(tp) \
do { \
    std::cout << tp.tv_sec << "," << tp.tv_nsec << std::endl; \
} \
while(false)

int main() {
    struct timespec tp;
    if (clock_gettime(CLOCK_REALTIME, &tp) != 0) {
        return -1;
    }
    PRINT_TP(tp);

    struct timespec monotic_tp;
    if (clock_gettime(CLOCK_MONOTONIC, &monotic_tp) != 0) {
        return -1;
    }
    PRINT_TP(monotic_tp);

    struct timespec monotic_raw_tp;
    if (clock_gettime(CLOCK_MONOTONIC_RAW, &monotic_raw_tp) != 0) {
        return -1;
    }
    PRINT_TP(monotic_raw_tp);
    return 0;
}