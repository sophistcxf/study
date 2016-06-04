/*************************************************************************
  > File Name: test_scoped_ptr.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Tue 31 May 2016 10:14:10 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include "boost/scoped_ptr.hpp"

class Obj
{
public:
  ~Obj() { cout << "~Obj" << endl; }
};

int testScopedPtr()
{
  cout << "before scoped" << endl;
  {
    boost::scoped_ptr<Obj> p1(new Obj());
    boost::scoped_ptr<Obj> p2(new Obj());
    boost::scoped_ptr<Obj> p3 = new Obj();
    cout << "before reset" << endl;
    p1.reset();
    cout << "after reset" << endl;
  }
  cout << "after scoped" << endl;
  return 0;
}
