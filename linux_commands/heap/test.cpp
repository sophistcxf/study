#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void test1()
{
    // 使用 heap 看时，可以看到分配的 2048 字节内存
    char* p = (char*)malloc(2048);
    printf("%p\n", p);
    getchar();
    delete [] p;
    // 由于 3070 不是 8 的整数倍，因而最终看到的是 3072 字节
    p = (char*)malloc(3070);
    printf("%p\n", p);
    getchar();
    delete [] p;
}

/*
 * 测试 sbrk 分配的内存
 */
void test2()
{
    char* p = (char*)sbrk(2048);
    printf("%p\n", p);
    getchar();
}

/*
 * 什么都不调用
 */
void test3()
{
    getchar();
}

int main(int argc, char* argv[])
{
    test3();
    return 0;
}
