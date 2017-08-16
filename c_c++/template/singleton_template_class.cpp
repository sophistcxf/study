/*************************************************************************
  > File Name: singleton_template_class.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Tue 18 Jul 2017 03:13:45 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

template <typename T>
class C1
{
public:
	void fun() { singleton.fun(); }
	static T singleton;
};

template <typename T>
T C1<T>::singleton;

class C2
{
public:
	void fun() { cout << "C2 fun" << endl; }
	C2() { cout << "C2" << endl; }
};

int main()
{
	C1<C2> c1;
	//c1.fun();
	return 0;
}
