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

int test1()
{
  int pipefd[2];
  if (pipe(pipefd) == -1) return -1;
  char buf[] = "hello world";
  write(pipefd[1], buf, sizeof(buf));
  char read_buf[128] = {0};
  read(pipefd[0], read_buf, sizeof(read_buf));
  printf("%s\n", read_buf);
}

int test2()
{
  int pipefd[2];
  if (pipe(pipefd) == -1) return -1;
  char read_buf[128] = {0};
  // read will be blocked
  read(pipefd[0], read_buf, sizeof(read_buf));
  return 0;
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

int test4()
{
  int pipefd[2];
  if (pipe(pipefd) == -1) return -1;
  if (fork() > 0) {
    // child process
    close(pipefd[0]);
    int i = 0;
    while (true) {
      char write_buf[128] = {0};
      snprintf(write_buf, 127, "This is %d\n", i++); 
      write(pipefd[1], write_buf, strlen(write_buf) + 1);
      sleep(1);
    }
  } else {
    // parent process
    close(pipefd[1]);
    while (true) {
      char read_buf[128] = {0};
      read(pipefd[0], read_buf, sizeof(read_buf));
      printf("%s", read_buf);
    }
  }
}

// pipe size
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

int main()
{
  test5();
  return 0;
}
