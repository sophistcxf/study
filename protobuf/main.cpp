/*************************************************************************
  > File Name: main.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Mon 20 Aug 2018 09:58:13 PM CST
 ************************************************************************/

#include <iostream>
#include <fstream>
using namespace std;

#include "person.idl.pb.h"

int main(int argc, char* argv[])
{
    int person_cnt = 1000;
    Class c;
    for (size_t i = 0; i < person_cnt; ++i)
    {
        c.set_class_name("class1");
        Person* p = c.add_persons();
        p->set_name("cxxxxf");
        p->set_age(i);
        p->set_address("address");
        p->set_num1(i);
        p->set_num2(i);
        p->set_num3(i);
        p->set_num4(i);
        p->set_num5(i);
        p->set_num6(i);
        p->set_num7(i);
        p->set_num8(i);
        p->set_num9(i);
        p->set_num10(i);
        p->set_num11(i);
        p->set_num12(i);
        p->set_num13(i);
    }
    std::string str;
    c.SerializeToString(&str);
    ofstream ofs("data.pb", ios_base::out|ios_base::binary);
    ofs << str;
    ofs.close();
    return 0;
}
