/*************************************************************************
  > File Name: test_ssize.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sun 10 Jul 2016 01:40:23 AM PDT
 ************************************************************************/

#include <iostream>
using namespace std;

int main()
{
  ssize_t i = 0xFFFFFFFF;
  cout << "sizeof(ssize_t): " << sizeof(ssize_t) << endl;
  cout << "sizeof(size_t): " << sizeof(size_t) << endl;
  cout << "sizeof(unsigned int): " << sizeof(unsigned int) << endl;
  cout << i << endl;
}
