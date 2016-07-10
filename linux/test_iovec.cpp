/*************************************************************************
  > File Name: test_iovec.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sun 10 Jul 2016 12:33:06 AM PDT
 ************************************************************************/

#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

#include <sys/uio.h>

int main()
{
  char part1[] = "This is iov";
  char part2[] = " and ";
  char part3[] = " writev test\n";
  struct iovec iov[3];
  iov[0].iov_base = part1;
  iov[0].iov_len = strlen(part1);
  iov[1].iov_base = part2;
  iov[1].iov_len = strlen(part2);
  iov[2].iov_base = part3;
  iov[2].iov_len = strlen(part3);
  writev(1, iov, 3);
  struct iovec in[3];
  in[0].iov_len = 5;
  in[1].iov_len = 4;
  in[2].iov_len = 5;
  readv(0, in, 3);
  fprintf(stderr, "in[0]: ");
  writev(1, &in[0], 1);
  printf("\n");
  printf("in[1]: ");
  writev(1, &in[1], 1);
  printf("\n");
  fprintf(stderr, "in[2]: ");
  writev(1, &in[2], 1);
  printf("\n");
  return 0;
}
