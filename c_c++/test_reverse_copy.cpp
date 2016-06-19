/*************************************************************************
  > File Name: test_reverse_copy.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sat 18 Jun 2016 09:32:12 PM PDT
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <string.h>
using namespace std;

int main()
{
  char a[] = "hello world!";
  char b[sizeof(a)];
  std::reverse_copy(a, a+strlen(a), b);
  cout << b;
  return 0;
}
