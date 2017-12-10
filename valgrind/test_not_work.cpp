/*************************************************************************
  > File Name: test_not_work.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Thu 30 Nov 2017 10:39:10 AM CST
 ************************************************************************/

#include <iostream>
#include <vector>
using namespace std;

class A
{
public:
    A(int c) : n(c) {}
    int n;
};

/*
 *能报出来，但我之前碰到过报不出来的情况
 */
int main()
{
    vector<A> v;
    v.push_back(A(0));
    A& p = v.back();
    for (int i = 1; i < 100; ++i)
    {
        v.push_back(A(i));
    }
    p.n = 1000;
    std::cout << v[0].n << std::endl;
}
