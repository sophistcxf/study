/*************************************************************************
  > File Name: test_join.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Mon 18 Mar 2019 03:08:05 PM CST
 ************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;


#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/lexical_cast.hpp>
#include "boost/lambda/lambda.hpp"


#if __cplusplus <= 199711L
template <typename T>
std::string to_string(T val)
{
    std::ostringstream stream;
    stream << val;
    return stream.str();
}
#else
#define to_string std::to_string
#endif

void simple_test()
{
    std::vector<std::string> v;
    v.push_back("zhangsan");
    v.push_back("beijing");
    v.push_back("student");
    std::string joined = boost::algorithm::join(v, "|");
    std::cout << joined << std::endl;
}

void simple_joinif_test()
{
    std::vector<std::string> v;
    v.push_back("zhangsan");
    v.push_back("beijing");
    v.push_back("student");
    v.push_back("male");
    std::string joined = boost::algorithm::join_if(v, "|", boost::lambda::_1.size() > 5);
    std::cout << joined << std::endl;
}

/*!
 * 如果是整型，首先要转一次
 */
void with_covert()
{
    std::vector<double> v;
    v.push_back(3.14);
    v.push_back(0.618);
    v.push_back(2.414);

    std::string joined = boost::algorithm::join(v| boost::adaptors::transformed(static_cast<std::string(*)(double)>(to_string)), "|"); 
    std::cout << joined << std::endl;
}

int main()
{
    //simple_test();
    simple_joinif_test();
}
