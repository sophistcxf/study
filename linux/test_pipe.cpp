/*************************************************************************
  > File Name: test_pipe.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Tue 19 Jul 2016 09:36:52 PM CST
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
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

int main()
{
  test3();
  return 0;
}
