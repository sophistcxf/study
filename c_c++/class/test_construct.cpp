#include <iostream>
using namespace std;

class A
{
public:
    A() : num(0)
    {
    }

	A(int i) : num(i) 
	{
		cout << "A::A" << endl;
	}
    virtual ~A()
    {
        cout << "A:~A" << endl;
    }
	int num;
};

void test1(const A& a)
{
	cout << a.num << endl;
}

void test2()
{
    void* p = new A;
    delete p;   // delete void*，行为是未定义的，并且 gcc 不调用析构函数

    p = new A;
    delete ((A*)p);
}

int main()
{
	// 解发隐式转换，调用A::A(int i)构造函数
	// test1(10);
    
    test2();

	return 0;
}
