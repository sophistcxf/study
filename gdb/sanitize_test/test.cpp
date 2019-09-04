#include <string.h>
#include <stdlib.h>

int main()
{
    const char* s = "Hello world";
    char* p = (char*)malloc(strlen(s));
    strcpy(p, s);
    return 0;
}
