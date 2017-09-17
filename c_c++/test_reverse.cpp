/*************************************************************************
  > File Name: test_reverse.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Mon 11 Sep 2017 03:57:39 PM CST
 ************************************************************************/

#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
 
int main()
{
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    std::reverse(v.begin(), v.end());
    std::cout << v[0] << v[1] << v[2] << '\n';
 
    int a[] = {4, 5, 6, 7};
    std::reverse(a, a + sizeof(a) / sizeof(int) - 1);
    std::cout << a[0] << a[1] << a[2] << a[3] << '\n';
}
