/*************************************************************************
  > File Name: main.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sun 17 Sep 2017 11:08:21 AM CST
 ************************************************************************/

#include <iostream>
#include <vector>
using namespace std;


int main()
{
    vector<int> v;
    for (unsigned i = 0; i < 10; ++i)
        v.push_back(i);
    for (unsigned i = 0; i < v.size(); ++i)
        cout << v[i] << "\n";
    return 0;
}
