/*************************************************************************
  > File Name: how_many.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Mon 15 Aug 2016 09:44:19 PM CST
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <unistd.h>
using namespace std;

void f()
{
  for (int i = 0; i < 2; ++i) {
    fork();
    printf("a\n");
  }
}

int main()
{
  f();
  return 0;
}
