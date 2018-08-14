/*************************************************************************
  > File Name: test_type_convert.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Mon 19 Mar 2018 08:09:30 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    int i = -1;
    unsigned ui = 10;
    //i将被转换成unsigned
    if (ui > i)
        cout << "ui > i" << "\t" << ui << "\t" << i << endl;
    else
        cout << "ui <= i" << "\t" << ui << "\t" << i << endl;
    return 0;
}
