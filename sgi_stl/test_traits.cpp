/*************************************************************************
  > File Name: test_traits.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sat 04 Jun 2016 05:08:08 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

template <class T>
struct traits
{
  traits() { cout << "traits" << endl; }
  void fun() { cout << "fun" << endl; }
  typedef typename T::value_type value_type;
};


// partial specialization
template <class T>
struct traits<T*>
{
  traits<T*>() { cout << "traits<T*>" << endl; }
  typedef T value_type;
};

int main()
{
  traits<string> t1;
  traits<int*> t2;
  t1.fun();
  // t2.fun(); t2 has no member function fun
  return 0;
};
