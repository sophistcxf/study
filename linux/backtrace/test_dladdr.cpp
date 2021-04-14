#include <stdio.h>
#include <dlfcn.h>

void test1()
{
    void* p = (void*)printf;
    printf("%p\n", p);

    Dl_info info;
    int rc = dladdr(p, &info);
    if (rc == 0) {
        printf("%p can't be found\n", p);
        return;
    }
    printf("dli_fname: %s\n", info.dli_fname);
    printf("dli_fbase: %p\n", info.dli_fbase);
    printf("dli_sname: %s\n", info.dli_sname);
    printf("dli_saddr: %p\n", info.dli_saddr);
}

int main()
{
    test1();
    return 0;
}
