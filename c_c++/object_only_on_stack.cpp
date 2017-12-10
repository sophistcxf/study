/*************************************************************************
  > File Name: object_only_on_heap.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 2017年02月19日 星期日 21时24分35秒
 ************************************************************************/

#include <iostream>
using namespace std;

class ObjectOnlyOnStack
{
public:
protected:
    void* operator new (size_t size);
};

int main()
{
    ObjectOnlyOnStack obj;
    // 不能在堆上申请
    // ObjectOnlyOnStack* pobj = new ObjectOnlyOnHeap();
    return 0;
}
