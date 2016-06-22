/*************************************************************************
  > File Name: test_cputimer.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 22 Jun 2016 06:55:30 PM CST
 ************************************************************************/

#include <iostream>
#include <cmath>
#include <boost/timer/timer.hpp>
using namespace std;

int main()
{
  boost::timer::cpu_timer timer;

  for (int i = 0; i < 1000000; ++i)
    std::pow(1.234, i);

  boost::timer::cpu_times times = timer.elapsed();
  std::cout << times.wall << '\n';
  std::cout << times.user << '\n';
  std::cout << times.system << '\n';
}
