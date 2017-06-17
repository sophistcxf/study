/*************************************************************************
  > File Name: judge_endian.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sat Jul  9 14:50:21 2016
 ************************************************************************/

#include <iostream>
#include <stdio.h>
using namespace std;

/*!
 * big endian: store most significant bit first (at low address)
 * little endian: store least significant bit first (at low address)
 */
bool isBigEndian()
{
  int i = 0x01020304;
  return *(char*)&i == 0x01;
}

int main()
{
  printf("%s\n", isBigEndian() ? "big endian" : "little endian");
  return 0;
}
