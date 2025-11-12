#include <cassert>
#include <iostream>
#include <queue>
#include <list>
#include <set>
 
void test1()
{
    std::queue<int> q;
 
    q.push(0); // back pushes 0
    q.push(1); // q = 0 1
    q.push(2); // q = 0 1 2
    q.push(3); // q = 0 1 2 3
 
    assert(q.front() == 0);
    assert(q.back() == 3);
    assert(q.size() == 4);
 
    q.pop(); // removes the front element, 0
    assert(q.size() == 3);
 
    // Print and remove all elements. Note that std::queue does not
    // support begin()/end(), so a range-for-loop cannot be used.
    std::cout << "q: ";
    for (; !q.empty(); q.pop())
        std::cout << q.front() << ' ';
    std::cout << '\n';
    assert(q.size() == 0);
}

void test2() {
    //模板参数1为模板参数2不一样，编译错误
    //std::queue<int, std::list<float> > q;

    std::queue<int, std::list<int> > q;

    q.push(0);
    q.push(1);
    for (; !q.empty(); q.pop()) {
        std::cout << q.front() << ' ';
    }
    std::cout << '\n';
}

void test3() {
    std::queue<int, std::list<int> > q;
    std::queue<float, std::vector<float> > q2;
    std::queue<double, std::deque<double> > q3;

}

void test_set() {
    // 只有q的定义这一句是，是可以编译的，但是加push，就会报错set没有push_back方法
    std::queue<int, std::set<int> > q;
    // q.push(1);
    // q.push(2);
}

int main() {
    test2();
    return 0;
}
