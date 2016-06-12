/*************************************************************************
  > File Name: test_bind1st.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sun 12 Jun 2016 02:21:46 PM CST
 ************************************************************************/

#include <iostream>
#include <functional>
#include <algorithm>
using namespace std;

int main () {
  int numbers[] = {10,20,30,40,50,10};
  int cx;
  cx = count_if (numbers, numbers+6, bind1st(equal_to<int>(),10) );
  cout << "There are " << cx << " elements that are equal to 10.\n";
  cx = count_if (numbers, numbers+6, bind1st(less<int>(), 30));
  cout << "there are " << cx << " elements that are bigger than 30.\n";
  cx = count_if (numbers, numbers+6, bind2nd(less<int>(), 30));
  cout << "there are " << cx << " elements that are less than 30.\n";
  return 0;
}
