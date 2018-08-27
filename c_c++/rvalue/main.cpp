/*************************************************************************
  > File Name: main.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Mon 27 Aug 2018 11:43:50 AM CST
 ************************************************************************/

#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

class People
{
public:
    People(std::string name) : name_(name)
    {
        std::cout << "People " << name_ << " construct!" << std::endl;
    }
    People(const People& p)
    {
        name_ = p.name_;
        std::cout << "People " << name_ << " copy construct!" << std::endl;
    }
    ~People()
    {
        std::cout << "People " << name_ << " destruct!" << std::endl;
    }
public:
    std::string name_;
};

void test1()
{
    std::vector<People> myvector;
    People p1("zhangsan");
    People p2("lisi");
    myvector.push_back(p1);                    // copies
    myvector.push_back(std::move(p2));         // moves
}

int main(int argc, char* argv[])
{
    test1();
    return 0;
}
