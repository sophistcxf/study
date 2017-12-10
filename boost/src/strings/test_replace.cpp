/*************************************************************************
  > File Name: test_replace.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 2017年02月06日 星期一 19时37分43秒
 ************************************************************************/

#include <iostream>
#include <string>
using namespace std;

#include <boost/algorithm/string/replace.hpp>

void test1();

int main()
{
    test1();
    return 0;
}

void test1()
{
    std::string a = "1_2_3_4";
    boost::replace_all(a, "_", ".");
    cout << a << endl;
}
