/*!
 * 测试各种情况下地址
 */

#include <stdio.h>

/*!
 * y 由于是引用，因而和 test1 中 y 的地址相同
 * &pc 比 &x 小 14B
 */
void foo(int x, int&y, int* pc)
{
    int z = 40; /* &z 比 &x 大 16 字节 */
    double d = 0.0; /* &d 比 &z 小 12 字节 */
    printf("address in foo:\n");
    printf("x address %x\n", &x);
    printf("y address %x\n", &y);
    printf("pc address %x\n", &pc);
    printf("*pc address %x\n", &(*pc));
    printf("z address %x\n", &z);
    printf("d address %x\n", &d);
}

void test1()
{
    int x = 10;
    int y = 20; /* &y 比 &x 小 4 */
    int* pc = new int;  /* &pc 比 &y 小 8 */
    *pc = 30;
    printf("address in test1:\n");
    printf("x address %x\n", &x);
    printf("y address %x\n", &y);
    printf("pc address %x\n", &pc);
    printf("*pc address %x\n", &(*pc));
    foo(x, y, pc);
}

int main()
{
    test1();
    return 0;
}
