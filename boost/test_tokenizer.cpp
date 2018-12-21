/*************************************************************************
  > File Name: test_tokenizer.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 08 Jun 2016 10:55:27 AM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include <boost/tokenizer.hpp>
using namespace boost; 

#define SEP "=================================================================================="

int main()
{
  string s = "This is, a test"; 
  tokenizer<> tok(s); 
  for(tokenizer<>::iterator beg=tok.begin(); beg!=tok.end();++beg) cout << *beg << endl; 
  cout << SEP;
  
  s = ";!!;Hello|world||-foo--bar;yow;baz|";
  tokenizer<char_separator<char> > tok1(s, char_separator<char>("-;|"));
  for (tokenizer<char_separator<char> >::iterator beg = tok1.begin(); beg != tok1.end(); ++beg) cout << *beg << endl;
}
