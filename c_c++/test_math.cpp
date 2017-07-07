/*************************************************************************
  > File Name: test_math.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 05 Jul 2017 05:43:30 PM CST
 ************************************************************************/

#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
using namespace std;

float logistic(float x)
{
	static const double e = 2.718281828459;
	return 1.0 / (1 + pow(e, x));
}

int main(int argc, char* argv[])
{
	int x = atoi(argv[1]);
	cout << logistic(x) << endl;
	return 0;
}
