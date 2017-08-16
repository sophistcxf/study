/*************************************************************************
  > File Name: test_template_func.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 16 Aug 2017 04:46:02 PM CST
 ************************************************************************/

/**
 * 测试使用函数作为模板参数
 */

#include <iostream>
using namespace std;

template <typename T, typename Func>
class Operator 
{
public:
	T operator() (const T& v1, const T& v2)
	{
		func(v1, v2);
	}
private:
	Func func;
};

template<typename T>
class Add
{
public:
	T operator()(const T& v1, const T& v2)
	{
		return v1 + v2;
	}
};

template<typename T>
class Sub 
{
public:
	T operator()(const T& v1, const T& v2)
	{
		return v1 - v2;
	}
};

int main()
{
	Operator<float, Add<float> > op_add;
	std::cout << op_add(1.1, 10.4) << endl;
	Operator<float, Sub<float> > op_sub;
	std::cout << op_sub(1.1, 10.4) << endl;
	return 0;
}
