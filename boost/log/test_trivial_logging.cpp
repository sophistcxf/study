/*************************************************************************
  > File Name: test_trivial_logging.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 15 Jun 2016 02:56:32 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include <boost/log/trivial.hpp>

int main()
{
  BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
  BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
  BOOST_LOG_TRIVIAL(info) << "An informational severity message";
  BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
  BOOST_LOG_TRIVIAL(error) << "An error severity message";
  BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
  return 0;
}
