/*************************************************************************
  > File Name: test_shared_ptr.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Fri 21 Dec 2018 03:02:42 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include <boost/shared_ptr.hpp>

int main()
{
    boost::shared_ptr<int> i(new int);
    *i.get() = 10;
    std::cout << i.use_count() << std::endl;
    boost::shared_ptr<int> i2 = i;
    std::cout << i.use_count() << std::endl;
    return 0;
}
