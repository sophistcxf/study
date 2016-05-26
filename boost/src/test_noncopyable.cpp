/*************************************************************************
  > File Name: test_noncopyable.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Thu 26 May 2016 10:12:14 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include "boost/noncopyable.hpp"

class NoncopyableClass : public boost::noncopyable {};

int testNoncopyable(int argc, char* argv[])
{
  NoncopyableClass c;
  // NoncopyableClass b = c; // can not copy noncopyable, error when compile 
  return 0;
}
