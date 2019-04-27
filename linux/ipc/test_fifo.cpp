#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;

extern int errno;

/*!
 * 在当前文件夹下，创建一个命名管道 abcd
 * 当命名管理已经存在时，再创建命名管理就会报错 EEXIST
 */
void test1()
{
    const char* path = "abcd";
    if (mkfifo(path, S_IRUSR) != 0) {
        std::cerr << "mkfifo error, errno is " << errno << std::endl;
        if (errno == EEXIST)
            std::cerr << "fifo is already exist" << std::endl;
        return;
    }
}

int main()
{
    test1();
}
