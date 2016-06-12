/*************************************************************************
  > File Name: test_bind.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sun 12 Jun 2016 02:12:10 PM CST
 ************************************************************************/

#include <iostream>
#include <functional>
#include <vector>
using namespace std;

class Obj
{
public:
  Obj(int i) : key(i) {}
    
public:
  int key;
};

int main()
{
  vector<Obj*> v;
  for (size_t i = 0; i < 10; ++i) v.push_back(new Obj(i));
  stable_sort(v.begin, v.end(), bind(less<int>, _1 ->* &Obj::key, _2 ->* &Obj::key));
  return 0;
}

