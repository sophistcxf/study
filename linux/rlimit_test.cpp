/*!
 *  测试 getrlimit, setrlimit
 */

#include <iostream>
#include <sys/resource.h>

void test_rlimit_core()
{
    struct rlimit rlp;
    getrlimit(RLIMIT_CORE, &rlp);
    std::cout << rlp.rlim_cur << "\t" << rlp.rlim_max << std::endl;
}

void test_specific_resource(int resource)
{
    struct rlimit rlp;
    getrlimit(resource, &rlp);
    std::cout << rlp.rlim_cur << "\t" << rlp.rlim_max << std::endl;
}

void test_rlimit_cpu()
{
    test_specific_resource(RLIMIT_CPU);
}

void test()
{
    test_rlimit_core();
    test_specific_resource(RLIMIT_CPU);
}

int main()
{
    test();
    return 0;
}
