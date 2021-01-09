#include <iostream>
#include <unistd.h>

extern int errno;

void test1()
{
    char *const argv[] = {"ls.sh", NULL};
    char *const environ[] = {NULL};
    int rlt = execve("ls.sh", argv, environ);
    switch (errno) {
        case EACCES:
            std::cout << "EACCES" << std::endl;
            break;
        case EFAULT:
            std::cout << "EFAULT" << std::endl;
            break;
        case ENOENT:
            std::cout << "ENOENT" << std::endl;
            break;
        case EIO:
            std::cout << "EIO" << std::endl;
            break;
        case E2BIG:
            std::cout << "E2BIG" << std::endl;
            break;
        case ENOMEM:
            std::cout << "ENOMEN" << std::endl;
            break;
        case ENOTDIR:
            std::cout << "ENOTDIR" << std::endl;
            break;
        case ETXTBSY:
            std::cout << "ETXTBSY" << std::endl;
            break;
        case ELOOP:
            std::cout << "ELOOP" << std::endl;
            break;
        case ENOEXEC:
            std::cout << "ENOEXEC" << std::endl;
            break;
        default:
            std::cout << errno << std::endl;
            break;
    }
}

int main()
{
    test1();
    return 0;
}
