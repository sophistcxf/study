/*************************************************************************
  > File Name: test_alloca.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Tue 28 Jun 2016 08:48:36 PM CST
 ************************************************************************/

#include <iostream>
#include <alloca.h>
using namespace std;

class A
{
public:
  A() { cerr << "A constructor" << endl; }
  int i;
};

int main()
{
  time_t b = time(NULL);
  for (int i = 0; i < 100000; ++i) {
    // do not call constructor
    // pa的作用域是整个函数，而非{}，但是不调用构造函数
    // 在构造函数效率低，又不想在堆上分配的场景中有用
    A* pa = (A*)alloca(sizeof(A));
    cerr << pa->i << ";" << i << "\t";
  }
  cout << time(NULL) - b << endl;
  for (int i = 0; i < 100000; ++i) {
    A pa;
    cerr << pa.i << ";" << i << "\t";
  }
  cout << time(NULL) - b << endl;
  return 0;
}
