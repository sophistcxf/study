#include <iostream>
using namespace std;

class A
{
public:
	A(int i) : num(i) 
	{
		cout << "A::A" << endl;
	}
	int num;
};

void test1(const A& a)
{
	cout << a.num << endl;
}

int main()
{
	// 解发隐式转换，调用A::A(int i)构造函数
	test1(10);
	return 0;
}
