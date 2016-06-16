/*************************************************************************
  > File Name: test_init_from_file.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 15 Jun 2016 09:23:55 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include <boost/log/utility/setup/from_stream.hpp>

namespace logging = boost::log;
int main(int, char*[])
{
  // Read logging settings from a file
  std::ifstream file("settings.ini");
  logging::init_from_stream(file);

  return 0;
}
