/*************************************************************************
  > File Name: test_placement_new.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sat 04 Jun 2016 03:56:20 PM CST
 ************************************************************************/
#include <iostream>
using namespace std;
class C
{
public:
  C() : i(10), str("hello world") {}
public:
  int i;
  string str;
};
int main()
{
  char* p = new char[100];
  C* pc = new(p) C;
  cout << pc->i << endl;
  cout << pc->str << endl;
  delete[] p;
  return 0;
}
