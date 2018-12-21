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

int add(int i, int j) { return i + j; }
int add_error(int i, int j) { return i + j + 1; }

BOOST_AUTO_TEST_CASE(test_add)
{
    BOOST_TEST(add(5, 10) == 15);
    BOOST_TEST(add(5, 10) == 14);

    if (add(5, 10) != 15)
    {
        BOOST_FAIL("Ouch Fail");
        BOOST_ERROR("Ouch Error");
    }
    if (add_error(5, 10) != 15)
    {
        //BOOST_FAIL导致停止运行
//        BOOST_FAIL("Ouch Fail");
        BOOST_ERROR("Ouch Error");
    }
    //BOOST_REQUIRE导致停止运行
    BOOST_REQUIRE(add(2,2)==3);
    BOOST_CHECK_MESSAGE(add(2,2)==3, "add(...) result: " << add(2,2));
    BOOST_CHECK_EQUAL(add(2,2), 3);
}

int subtract(int i, int j) { return i - j; }
int subtract_error(int i, int j) { return i - j - 1; }

BOOST_AUTO_TEST_SUITE(test_suite1)
BOOST_AUTO_TEST_CASE(test_subtract)
{
    BOOST_TEST(subtract(5, 10) == -5);
    BOOST_TEST(subtract_error(5, 10) == -5);
}

BOOST_AUTO_TEST_SUITE_END()
    
