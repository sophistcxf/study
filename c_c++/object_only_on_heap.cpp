/*************************************************************************
  > File Name: object_only_on_heap.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 2017年02月19日 星期日 21时24分35秒
 ************************************************************************/

#include <iostream>
using namespace std;

class ObjectOnlyInHeap
{
public:
    void Destroy()
    {
        // 成员函数可以访问protected/private
        // 因而可以析构掉
        delete this;
    }
protected:
    ~ObjectOnlyInHeap(){};
};

int main()
{
    // 不能在栈上构造
    // ObjectOnlyInHeap obj;
    ObjectOnlyInHeap* pobj = new ObjectOnlyInHeap();
    // 此时也不能调用delete
    // delete pobj
    pobj->Destroy();
    return 0;
}
