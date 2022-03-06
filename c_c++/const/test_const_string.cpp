#include <stdio.h>

/**
 * 字符串i'm global位于.data中
 */
char g_str[] = "i'm global";

/**
 * g_ro_str是位于.data中
 * 字符串i'm read only global位于.rodata中
 */
char* g_ro_str = "i'm read only global";

void test1() {
    /**
     * str1的修改是ok的
     */
    char str1[] = "hello";
    printf("%c\n", str1[0]);
    str1[0] = 'a';
    printf("&str1,&str1[0],&str1[1]: %x,%x,%x,%x\n", &str1, &str1[0], &str1[1]);

    char* str = "hello";
    printf("&str,&str[0]: %x,%x\n", &str, &str[0]);
    printf("%c\n", str[0]);

    char str2[] = "world";
    printf("&str2: %x\n", &str2);

    printf("&g_str,&g_str[0]: %x,%x\n", &g_str, &g_str[0]);
    printf("&g_ro_str,&g_ro_str[0]: %x,%x\n", &g_ro_str, &g_ro_str[0]);

    /**
     * 对str[0]进行修改，会导致SIGSEGV
     */
    str[0] = 'a';   

    /**
     * 从str1, str1[0], str, str[0]的地址来看，str[0]并不再栈上
     * &str1, &str1[0]: 2867bad0,2867bad0
     * &str, &str[0]: 2867bac8,40069b
     */
}

int main() {
    test1();
    return 0;
}
