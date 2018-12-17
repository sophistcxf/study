/*************************************************************************
  > File Name: test_if_then.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 2016年09月02日 星期五 13时45分00秒
 ************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#include <boost/lambda/lambda.hpp>
#include <boost/phoenix/phoenix.hpp>

void test_1();

int main()
{
  test_1();
}

void test_1()
{
  vector<int> v{1, 3, 2, -1, -10, 3};
  using namespace boost::lambda;
  std::sort(v.begin(), v.end(), _1 < _2);
  for (unsigned i = 0; i < v.size(); ++i)
    cout << v[i] << " ";
}
