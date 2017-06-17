/*************************************************************************
  > File Name: example_falsify_printf.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sat 06 Aug 2016 05:41:39 PM CST
 ************************************************************************/

#include <iostream>
#include <cstdio>
using namespace std;

int main(int argc, char *argv[])
{
  int a = 1, b = 2;
  if (a != b) {
    printf("Sorry!\n");
    return 0;
  }
  printf("OK!\n");
  return 1;
}
