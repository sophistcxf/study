/*************************************************************************
  > File Name: test_split.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 08 Jun 2016 11:56:21 AM CST
 ************************************************************************/

#include <iostream>
#include <vector>
using namespace std;

#include "boost/algorithm/string.hpp"

void test1()
{
  string str = "hello world, I'm xiaoming";
  vector<string> v;
  boost::split(v, str, boost::is_any_of(" "));
  for (size_t i = 0; i < v.size(); ++i)
      cout << v[i] << endl;
}

void test2()
{
    string str = "a>10:b=1|2|3:c<=5";
    std::vector<std::string> comparison_ops;
    comparison_ops.push_back(">");
    comparison_ops.push_back(">=");
    comparison_ops.push_back("<");
    comparison_ops.push_back("<=");
    comparison_ops.push_back("=");
    std::vector<std::string> fds;
    boost::split(fds, str, boost::is_any_of(":"));
    for (std::vector<std::string>::iterator it = fds.begin(); it != fds.end(); ++it) {
        std::vector<std::string> tri_set;
        boost::split(fds, *it, comparison_ops);
        if (fds.size() != 2) {
            std::cerr << "trible set is not equal 3, " << *it << std::endl;
            continue;
        }
        std::string variate = fds[0];
        std::string value = fds[1];
        std::cout << variate << value << std::endl;
    }
}

int main()
{
    test2();
    return 0;
}
