/*************************************************************************
  > File Name: test_replace.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 15 Jun 2016 10:23:38 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include <boost/algorithm/string/replace.hpp>
using namespace boost::algorithm;

int main()
{
  std::string format = "format";
  std::string search = "%d";
  std::string input = "%d|%s|%p|%m";
  replace_all(input, search, format);
  cout << input << endl;
  return 0;
}
