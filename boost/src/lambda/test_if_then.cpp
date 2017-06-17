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
#include <boost/lambda/if.hpp>

void test_1();

int main()
{
  test_1();
}

void test_1()
{
  vector<int> v{1, 3, 2, -1, -10, 3};
  int positive_cnt = 0;
  std::for_each(v.begin(), v.end(), boost::lambda::if_then(boost::lambda::_1 > 0, std::cout << boost::lambda::_1 << "\n"));
  cout << positive_cnt << endl;
}
