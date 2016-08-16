/*************************************************************************
  > File Name: test_mkdir.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Fri 12 Aug 2016 11:34:36 AM CST
 ************************************************************************/

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;

int main()
{
  mkdir("cxxxxf", 0777);
  return 0;
}
