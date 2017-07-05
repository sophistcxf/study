/*************************************************************************
  > File Name: header.h
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 05 Jul 2017 11:48:01 AM CST
 ************************************************************************/

#include <iostream>
#include <map>
#include <string>
using namespace std;

class CA
{
public:
	unsigned getA()
	{
		return a;
	}
private:
	static const unsigned a = 10;
	// 一定要在cpp中定义
	static unsigned i;
};

template <typename T>
class CB
{
public:
	struct InnerCB
	{
		int inner;
	};
	static unsigned i;
	static map<string, InnerCB> m;
};

int f1();
int f2();
