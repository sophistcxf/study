#include <iostream>
#include <string>
#include <list>
#include <deque>
 
struct A
{
    std::string s;
 
    A(std::string str) : s(std::move(str)) { std::cout << " constructed\n"; }
 
    A(const A& o) : s(o.s) { std::cout << " copy constructed\n"; }
 
    A(A&& o) : s(std::move(o.s)) { std::cout << " move constructed\n"; }
 
    A& operator=(const A& other)
    {
        s = other.s;
        std::cout << " copy assigned\n";
        return *this;
    }
 
    A& operator=(A&& other)
    {
        s = std::move(other.s);
        std::cout << " move assigned\n";
        return *this;
    }
};
 
int main()
{
    std::list<A> container;
 
    // 只构造一次
    std::cout << "emplace:\n";
    container.emplace(container.end(), "one");
 
    // 需要构造两次
    std::cout << "emplace with A&:\n";
    A two { "two" };
    container.emplace(container.end(), two);
 
    std::cout << "emplace with A&&:\n";
    A three { "three" };
    std::cout << "before move construct " << three.s << std::endl;
    container.emplace(container.end(), std::move(three));
    // 经过移动构造之后，three的状态不再可用
    std::cout << "after move construct " << three.s << std::endl;
}