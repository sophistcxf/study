/*************************************************************************
  > File Name: test_pipe.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Tue 19 Jul 2016 09:36:52 PM CST
 ************************************************************************/

#define _GNU_SOURCE 

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
using namespace std;

extern int errno;

/*!
 * 简单的示例，使用 pipe 打开两个文件描述符，往 pipefd[1] 写，从 pipefd[0] 读
 */
int test1()
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
        return -1;
    char* buf = "hello world";
    write(pipefd[1], buf, sizeof(buf));
    char read_buf[128] = {0};
    read(pipefd[0], read_buf, sizeof(read_buf));
    printf("%s\n", read_buf);

    buf = "I'm cxxxxf";
    write(pipefd[0], buf, sizeof(buf));
    read(pipefd[1], read_buf, sizeof(read_buf));
    printf("%s\n", read_buf);
}

/*!
 * 当没有往 pipefd[1] 写时，直接读 pipefd[0]，则会阻塞，可以使用 fcntl 将 pipefd[0]
 * 设置为非阻塞
 */
int test2()
{
    int pipefd[2];
    if (pipe(pipefd) == -1) return -1;
    char read_buf[128] = {0};
    // read will be blocked
    read(pipefd[0], read_buf, sizeof(read_buf));
    return 0;
}

/*!
 * 如果往 pipefd[0] 写，从 pipefd[1] 读，会发生什么？
 * 从 pipefd[1] 读时直接报错，errno=9，EBADF，不是一个有效的文件，或者不为写打开
 * 往 pipefd[0] 写时，errno=9，EBADF，不是一个有效的文件，或者不为写打开
 */
int test6()
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
        return -1;
    char read_buf[128] = {0};
    ssize_t size = read(pipefd[1], read_buf, sizeof(read_buf));
    if (size < 0)
        std::cerr << "read error, errno is " << errno << std::endl;
    std::cout << size << "\t" << read_buf << endl;
    char write_buf[] = "Hello world";
    size = write(pipefd[0], write_buf, sizeof(write_buf));
    if (size < 0)
        std::cerr << "write error, errno is " << errno << std::endl;
    std::cout << size << std::endl;
}

int test3()
{
    int pipefd[2];
    if (pipe(pipefd) == -1) return -1;
    char write_buf[128] = {0};
    int i = 0;
    while (true) {
        snprintf(write_buf, 127, "This is %d\n", i++); 
        printf("%s", write_buf);
        write(pipefd[1], write_buf, strlen(write_buf) + 1);
    }
    return 0;
}

/*!
 * 父子进程间通信
 */
int test4()
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
        return -1;
    if (fork() > 0) {
        // 子进程，关闭读描述符
        close(pipefd[0]);
        int i = 0;
        while (true) {
            char write_buf[128] = {0};
            snprintf(write_buf, 127, "This is %d\n", i++); 
            write(pipefd[1], write_buf, strlen(write_buf) + 1);
            sleep(1);
        }
    } else {
        // 父进程，关闭写描述符 
        close(pipefd[1]);
        while (true) {
            char read_buf[128] = {0};
            read(pipefd[0], read_buf, sizeof(read_buf));
            printf("%s", read_buf);
        }
    }
}

/*!
 * 如果有两个子进程，同时写入
 */
int test7()
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
        return -1;
    if (fork() > 0) {
        // 子进程，关闭读描述符
        close(pipefd[0]);
        int i = 0;
        while (true) {
            char write_buf[128] = {0};
            snprintf(write_buf, 127, "I'm child1, This is %d\n", i++); 
            write(pipefd[1], write_buf, strlen(write_buf) + 1);
            sleep(1);
        }
    } else {
        // 再 fork 一个子进程
        if (fork() > 0) {
            // 第二个子进程，关闭读描述符
            close(pipefd[0]);
            int i = 0;
            while (true) {
                char write_buf[128] = {0};
                snprintf(write_buf, 127, "I'm child2, This is %d\n", i++); 
                write(pipefd[1], write_buf, strlen(write_buf) + 1);
                sleep(1);
            }
        }
        else {
            close(pipefd[1]);
            while (true) {
                char read_buf[128] = {0};
                read(pipefd[0], read_buf, sizeof(read_buf));
                printf("%s", read_buf);
            }
        }
    }
}

// pipe size
/*
int test5()
{
    int pipefd[2];
    if (pipe(pipefd) == -1) return -1;
    struct utsname kernel_info;
    if (uname(&kernell_info) < 0) return -1;

    int ret = fcntl(pipefd[1], F_GETPIPE_SZ);  
    if (ret < 0) return -1;
    printf("PIPESIZE: %d\n", ret);
    return 0;
}
*/

int main()
{
    //test2();
    //test6();
    //test4();
    test7();
    return 0;
}
