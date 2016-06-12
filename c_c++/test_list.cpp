/*************************************************************************
  > File Name: test_list.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sat 04 Jun 2016 06:27:39 PM CST
 ************************************************************************/

#include <iostream>
#include <list>
using namespace std;

class A
{
public:
  A() : i(100) { cout << "A" << endl; }
  int i;
};

class C
{
public:
  int i;
  C() : i(10) { cout << "C" << endl; pa = new A(); }
  ~C() { delete pa; pa = NULL; cout << "~C" << endl; }
  C(const C& c) : i(c.i) { cout << "C(const C& c)" << endl; }
  C& operator=(const C& c) { cout << " C& operator=(const C& c)" << endl; }
  A* pa;
private:
};

list<C> l;

void fun()
{
  C c;
  l.push_back(c);
  c.i = 20;
}

int main()
{
  fun();
  for (list<C>::iterator iter = l.begin(); iter != l.end(); iter++)
    cout << (*iter).i << endl;
  C& c = *(l.end());
  l.clear();
  return 0;
}

