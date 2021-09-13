#include <string.h>
#include <stdio.h>

void test1() {
    char a[] = "hello world";
    char* b = a + 3;
    memcpy(b, a, 3);
    printf("%s\n", b);

    char a1[] = "hello world";
    b = a1 + 3;
    memmove(b, a1, 3);
    printf("%s\n", b);
}

int main() {
    test1();
    return 0;
}
