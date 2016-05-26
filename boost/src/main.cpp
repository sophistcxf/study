/*************************************************************************
  > File Name: main.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Thu 26 May 2016 10:12:22 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

int testNoncopyable();
int testLexicalcase();

int main(int argc, char* argv[])
{
  //testNoncopyable(argc, argv);
  testLexicalcase();
  return 0;
}
