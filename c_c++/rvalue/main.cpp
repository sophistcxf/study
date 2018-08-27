/*************************************************************************
  > File Name: main.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Mon 27 Aug 2018 11:43:50 AM CST
 ************************************************************************/

#include <iostream>
#include <string>
using namespace std;

class People
{
public:
    People(std::string name) : name_(std::move(name)) {}
public:
    std::string name_;
};

int main(int argc, char* argv[])
{
    People a("Alice");
    string bn = "Bob";
    People b(bn);
    return 0;
}
