/*************************************************************************
  > File Name: falsify_strcmp.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sat 06 Aug 2016 05:33:09 PM CST
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

int main(int argc, char *argv[])
{
  char passwd[] = "password";
  if (argc < 2) return -1;
  if (strcmp(argv[1], passwd) == 0)
    printf("Corrent\n");
  else
    printf("Invalid\n");
  return 0;
}
