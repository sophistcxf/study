/*************************************************************************
  > File Name: test_distance.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sat 18 Jun 2016 09:19:27 PM PDT
 ************************************************************************/

#include <iostream>
#include <iterator>
#include <list>
using namespace std;

int main()
{
  char a[] = "hello world!";
  cout << std::distance(a+1, a+10);
  list<int> l;
  for (int i = 0; i < 10; ++i) l.push_back(i);
  cout << std::distance(l.begin(), l.end());
  return 0;
}
