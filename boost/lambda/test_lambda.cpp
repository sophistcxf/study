/*************************************************************************
  > File Name: test_lamda.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sun 12 Jun 2016 10:06:10 AM CST
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

#include "boost/lambda/lambda.hpp"
#include "boost/function.hpp"

void test_1();
void test_2();

int main()
{
    test_1();
    return 0;
}

void test_1()
{
  (std::cout << boost::lambda::_1<< " " 
             << boost::lambda::_2 << " " 
             << boost::lambda::_3 << " "<<std::endl)("Hello", "friend", "my");
  boost::function<void(const std::string&, const std::string&, const std::string&)> f = (std::cout << boost::lambda::_1 << " "
                                                                               << boost::lambda::_2 << " "
                                                                               << boost::lambda::_3 << " "
                                                                               << std::endl);
  f("Hello", "friend", "my");
} 

class C
{
  public:
    C(int n) : num(n) {}
    int num;
};
/*
void test_2()
{
  std::vector<C> vec;
  vec.push_back(C(10));
  vec.push_back(C(5));
  vec.push_back(C(7));
  vec.push_back(C(3));

  boost::function<void(const C&, const C&)> f = return _1.num < _2.num;
  std::stable_sort(vec.begin(), vec.end(), boost::lambda::_1.num > boost::lambda::_2.num);
}
*/
