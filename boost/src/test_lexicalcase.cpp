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

int testLexicalcast()
{
  using namespace boost;
  int x = lexical_cast<int>("100");
  long y = lexical_cast<long>("2000");
  string str = lexical_cast<string>(100);
  cout << x << "\t" << y << "\t" << str << "\t" << lexical_cast(0.618) << endl;
  return 0;
}
