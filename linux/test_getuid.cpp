/*************************************************************************
  > File Name: test_getuid.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Mon Jul  4 21:44:38 2016
 ************************************************************************/

#include <iostream>
using namespace std;

#include <sys/types.h>
#include <unistd.h>

int main()
{
  cout << "real user id: " << getuid() << endl;
  cout << "effective user id: " << geteuid() << endl;
  return 0;
}
