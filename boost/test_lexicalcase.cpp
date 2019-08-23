/*************************************************************************
  > File Name: test_lexicalcast.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Thu 26 May 2016 11:07:13 PM CST
 ************************************************************************/

#include <iostream>
#include <string>
using namespace std;

#include "boost/lexical_cast.hpp"

int test()
{
  using namespace boost;
  int x = lexical_cast<int>("100");
  long y = lexical_cast<long>("2000");
  string str = lexical_cast<string>(100);
  cout << x << "\t" << y << "\t" << str << endl;

  return 0;
}

void test2()
{
  std::string f_str = "21.2345";
  boost::lexical_cast<int>(f_str);
}

int main()
{
    //test();
    test2();
    return 0;
}
