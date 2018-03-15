/*************************************************************************
  > File Name: test_fixture.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Thu 15 Mar 2018 07:51:49 PM CST
 ************************************************************************/


#include <iostream>
#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

struct F {
    F() : i( 0 ) { BOOST_TEST_MESSAGE( "setup fixture" ); }
    ~F() { BOOST_TEST_MESSAGE( "teardown fixture" ); }
    int i;
};

BOOST_FIXTURE_TEST_CASE( test_case1, F )
{
    BOOST_CHECK( i == 1 );
    //++i只影响本test case，不影响test_case2
    ++i;
    BOOST_CHECK_EQUAL(i, 2);
}

BOOST_FIXTURE_TEST_CASE( test_case2, F )
{
        BOOST_CHECK_EQUAL( i, 1 );
}

//这样场景F f也可以使用在多个test case中
F f;
BOOST_AUTO_TEST_CASE( test_case3 )
{
    BOOST_CHECK_EQUAL(f.i, 1);
    BOOST_CHECK( true );
}

//这样F是影响全局的
BOOST_FIXTURE_TEST_SUITE(s, F)

BOOST_AUTO_TEST_CASE(test_case4)
{
    BOOST_CHECK( i == 1 );
    i++;
}

BOOST_AUTO_TEST_CASE(test_case5)
{
    BOOST_CHECK_EQUAL( i, 0 );
}

BOOST_AUTO_TEST_SUITE_END()

struct MyConfig {
    MyConfig()   { j=0; std::cout << "global setup\n"; }
    ~MyConfig()  { std::cout << "global teardown\n"; }
};

//MyConfig的构造和析构函数分别在测试开始和结束时调用一次
//在测试过程中，是否可以引用MyConfig中的成员
BOOST_GLOBAL_FIXTURE( MyConfig );

BOOST_AUTO_TEST_CASE( test_case )
{
    BOOST_CHECK( true );
}
