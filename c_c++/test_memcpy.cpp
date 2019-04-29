/*************************************************************************
  > File Name: test_memcpy.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Mon 29 Apr 2019 05:32:15 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

struct Individual
{
public:
    int age;
};

void* self_memcpy(void* dst, const void* src, size_t n)
{
    if (dst == NULL || src == NULL)
        return NULL;
    char* tmp = (char*)dst;
    const char* src_tmp = (const char*)src;
    while (n--)
        *tmp++ = *src_tmp++;
    return dst;
}

void test1()
{
    Individual indi;
    indi.age = 20;
    Individual indi2;
    self_memcpy(&indi2, &indi, sizeof(Individual));
    std::cout << indi2.age << std::endl;
}

int main()
{
    test1();
    return 0;
}
