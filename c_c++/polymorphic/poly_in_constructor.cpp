/*************************************************************************
  > File Name: poly_in_constructor.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Tue 08 Aug 2017 05:58:15 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

class BaseC
{
public:
	virtual ~BaseC() 
	{
		cout << "BaseC::~BaseC" << endl;
		fun1();
	}
	virtual void fun1()
	{
		cout << "BaseC::fun1" << endl;
	}
	virtual void v_fun() = 0;
};

class DerivedC : public BaseC
{
public:
	virtual ~DerivedC()
	{
		cout << "DerivedC::~DerivedC" << endl;
		fun1();
		v_fun();
	}
	virtual void fun1()
	{
		cout << "DerivedC::fun1" << endl;
	}
	virtual void v_fun()
	{
		cout << "DerivedC::v_fun" << endl;
	}
};

int main()
{
	BaseC* pB = new DerivedC();
	delete pB;
	return 0;
}
