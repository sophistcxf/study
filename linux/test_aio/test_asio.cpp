/*************************************************************************
  > File Name: test_asio.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 2016年08月24日 星期三 20时38分19秒
 ************************************************************************/

#include <iostream>
#include <errno.h>
#include <aio.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

int main()
{
  struct aiocb aio;
  int fd = open("text", O_RDONLY);
  aio.aio_fildes = fd;
  aio.aio_offset = 0;
  aio.aio_buf = new char[30];
  memset((char*)aio.aio_buf, 0, 30);
  aio.aio_nbytes = 30;
  aio.aio_reqprio = 0;
  if (aio_read(&aio) < 0) 
    printf("aio_read(&aio) fail.\n");

  printf("aio_read end.\n");
  //sleep(5);
  while (true) {
    int rlt = aio_error(&aio);
    if (rlt == 0) {
      printf("%s\n", aio.aio_buf);
      break;
    }
    else if (rlt == EINPROGRESS) {
      printf("read is still awaiting, sleeping for 1s.\n");
      sleep(1);
      continue;
    }
    else if (rlt == -1) 
      printf("aio_read error.\n");
    else 
      printf("aio_read error.\n");
  }
  return 0;
}
