#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*
 * 测试 brk/sbrk 的基本功能
 * 尽量不使用 glibc 会影响内存情况的函数
 */
int test1()
{
    /*
     * size 是 1024 和 1024 * 1024 时，VSZ 和 RSS 的变化会有所不同
     * size == 1024 时，虽然只申请了 1kB，但是 VSZ 会增加 4kB，应该是因为页是最小分配单元
     */
    int size = 1024;
    // 1
    getchar();
    fflush(stdin);
    write(1, "call sbrk(1024)\n", 16);
    void* start_brk = sbrk(0);
    sbrk(size);
    // 2
    char* now_brk = (char*)sbrk(0);
    getchar();
    write(1, "call memset\n", 12);
    /*
     * now_brk 比 start_brk 大 1024
     * 堆涨的时候，是从小地址往大地址涨
     */
    char buf[256] = {0};
    snprintf(buf, 256, "start_brk: %p, now_brk: %p\n", start_brk, now_brk);
    write(1, buf, strlen(buf));
    memset(start_brk, 0x64, size);
    // 3
    getchar();
    return 0;
}

/*
 * 测试 malloc 是否调用 brk/sbrk 分配内存 
 * 使用 strace 查看系统调用
 */
int test2()
{
    getchar();
    char* p = (char*)malloc(64);    // 会调用 brk
    memset(p, 0, 64);           // memset 无系统调用
    strcpy(p, "hello world");   // strcpy 无系统调用
    printf("%s, 0x%p\n", p, p);
    delete [] p;
}

/*
 * 测试异常参数
 */
void test3()
{
    getchar();
    brk(0);
}

int main(int argc, char* argv[])
{
    test1();
    return 0;
}
