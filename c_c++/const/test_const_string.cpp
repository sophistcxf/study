#include <stdio.h>

void test1() {
    char* str = "hello";
    printf("%c\n", str[0]);
    str[0] = 'a';   
}

int main() {
    test1();
    return 0;
}
