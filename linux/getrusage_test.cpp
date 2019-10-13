/*************************************************************************
  > File Name: getrusage_test.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sun Oct 13 10:50:37 2019
 ************************************************************************/

#include <iostream>
using namespace std;

#include <sys/resource.h>

void output_rusage(const struct rusage* ru)
{
    std::cout << "user time used: " << ru->ru_utime.tv_usec << std::endl
              << "system time used: " << ru->ru_stime.tv_usec << std::endl
              << "max resident set size: " << ru->ru_maxrss << std::endl
              << "integral shared text memory size: " << ru->ru_ixrss << std::endl
              << "integral unshared data size: " << ru->ru_idrss << std::endl
              << "integral unshared stack size: " << ru->ru_isrss << std::endl
              << "page reclaims: " << ru->ru_minflt << std::endl
              << "page faults: " << ru->ru_majflt << std::endl
              << "swaps: " << ru->ru_nswap << std::endl
              << "block input operations: " << ru->ru_inblock << std::endl
              << "block output operations: " << ru->ru_oublock << std::endl
              << "messages sent: " << ru->ru_msgsnd << std::endl
              << "messages received: " << ru->ru_msgrcv << std::endl
              << "signals received: " << ru->ru_nsignals << std::endl
              << "voluntary context switches: " << ru->ru_nvcsw << std::endl
              << "involuntary context switches: " << ru->ru_nivcsw << std::endl;
}

void test1()
{
    struct rusage ru;
    int rlt = getrusage(RUSAGE_SELF, &ru);
    output_rusage(&ru);
}

int main()
{
    test1();
    return 0;
}
