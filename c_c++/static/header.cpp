/*************************************************************************
  > File Name: header.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 05 Jul 2017 03:15:01 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include "header.h"

template <typename T>
unsigned int CB<T>::i = 0;
template <typename T>
map<string, typename CB<T>::InnerCB> CB<T>::m;

int f1()
{
	CB<int> b;
	cout << b.i << endl;
	cout << b.m.size() << endl;
	return 0;
}
