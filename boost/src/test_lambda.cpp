/*************************************************************************
  > File Name: test_lamda.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sun 12 Jun 2016 10:06:10 AM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include "boost/lambda/lambda.hpp"
#include "boost/function.hpp"

int main()
{
  using namespace boost::lambda;
  (std::cout << _1 << " " << _2 << " " << _3 << " ")("Hello", "friend", "my");
  return 0;
}
