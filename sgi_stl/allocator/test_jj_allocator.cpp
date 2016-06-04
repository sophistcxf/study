/*************************************************************************
  > File Name: test_jj_allocator.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sat 04 Jun 2016 03:06:37 PM CST
 ************************************************************************/

#include <iostream>
#include <vector>
using namespace std;

#include "jj_allocator.h"

int main()
{
  int ia[5] = {0, 1, 2, 3, 4};
  unsigned i;
  vector<int, JJ::allocator<int> > iv(ia, ia+5);
  for (i = 0; i < iv.size(); i++) cout << iv[i] << endl;
  return 0;
}

