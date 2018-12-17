/*************************************************************************
  > File Name: test_auto_cputimer.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 22 Jun 2016 08:01:27 PM CST
 ************************************************************************/

#include <iostream>
#include <cmath>
using namespace std;

#include <boost/timer/timer.hpp>
using namespace boost::timer;

int main()
{
  auto_cpu_timer timer;

  for (int i = 0; i < 1000000; ++i)
    std::pow(1.234, i);
}
