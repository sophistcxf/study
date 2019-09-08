#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char* argv[])
{
    test1();
    return 0;
}
