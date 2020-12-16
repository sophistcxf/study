#include <string.h>
#include <stdlib.h>
#include <vector>
#include <iostream>


void test1()
{
    const char* s = "Hello world";
    std::cout << strlen(s) << std::endl;
    char* p = (char*)malloc(strlen(s));
    strcpy(p, s);
}

void test2()
{
    std::vector<int> v;
    for (int i = 0; i < 100; ++i)
        v.push_back(i);
    std::cout << v.capacity() * sizeof(int) << std::endl;
    for (int i = 0; i < v.size() + 1; ++i)
        std::cout << v[i] << std::endl;
}

void test3()
{
    char* p = (char*)malloc(64);
    p[64] = '\0';
    std::cout << p << std::endl;
}

void test4()
{
    const char* s = "Hello world";
    char* src = (char*)malloc(strlen(s)+1);
    strcpy(src, s);
    char* dst = (char*)malloc(strlen(s)+64);
    memcpy(dst, src, strlen(src)+64);
}

int main()
{
    test4();
    return 0;
}
