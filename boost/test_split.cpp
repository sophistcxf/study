/*************************************************************************
  > File Name: test_split.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 08 Jun 2016 11:56:21 AM CST
 ************************************************************************/

#include <iostream>
#include <vector>
using namespace std;

#include <boost/algorithm/string.hpp>

int main()
{
  string str = "hello world, I'm xiaoming";
  vector<string> v;
  boost::split(v, str, boost::is_any_of(" "));
  for (size_t i = 0; i < v.size(); ++i) cout << v[i] << endl;
  return 0;
}
