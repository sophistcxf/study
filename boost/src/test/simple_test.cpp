/*************************************************************************
  > File Name: simple_example.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Tue 28 Jun 2016 11:40:59 AM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#define BOOST_TEST_MODULE My Test
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(first_test)
{
  int i = 1;
  BOOST_TEST(i);
  BOOST_TEST(i == 2); 
  int a = 1, b = 2;
  namespace tt = boost::test_tools;
}
