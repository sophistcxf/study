/*************************************************************************
  > File Name: test_throw.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Mon 25 Jul 2016 07:55:39 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

// can't throw any exception
// lead to core dump 
void foo() throw() 
{
  throw 10;
}

void foo2()
{
  throw 10;
}

int main()
{
  try {
    foo();
    foo2();
  }
  catch(...) {
    cout << "catch ..." << endl;
  }
}
