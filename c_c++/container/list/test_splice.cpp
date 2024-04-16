#include <iostream>
#include <list>
 
std::ostream& operator<<(std::ostream& ostr, const std::list<int>& list)
{
    for (auto& i : list)
        ostr << ' ' << i;
 
    return ostr;
}
 
int main ()
{
    std::list<int> list1{1, 2, 3, 4, 5};
    std::list<int> list2{10, 20, 30, 40, 50};
 
    auto it = list1.begin();
    std::advance(it, 2);
 
    // 将 list2 全部插入到 begin()+2 前面
    list1.splice(it, list2);
 
    std::cout << "list1:" << list1 << '\n';
    std::cout << "list2:" << list2 << '\n';
 
    // 将list1的 it 之前的都插入到list2
    // 注意，这里的it仍然是有效的
    list2.splice(list2.begin(), list1, it, list1.end());
 
    std::cout << "list1:" << list1 << '\n';
    std::cout << "list2:" << list2 << '\n';
}