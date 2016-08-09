/*************************************************************************
  > File Name: test_union.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Wed 03 Aug 2016 02:05:32 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

union
{
 int ui;
 char c[4];
 bool ub;
} u1;


int main()
{
  cout << sizeof(u1) << endl;
  u1.ui = 10;
  cout << u1.ui << endl;
  cout << u1.ub << endl;
  cout << u1.c[0] << endl;
  return 0;
}
