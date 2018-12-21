/*************************************************************************
  > File Name: test_flyweight.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 2017年02月03日 星期五 11时49分39秒
 ************************************************************************/

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
using namespace std;

#include <boost/flyweight.hpp>

void WithFlyWeight()
{
    char tmp[10];  
    vector< boost::flyweight<string> >  boost_v;  

    for(int i =0;i< 10000000;i++)  
    {  
        sprintf(tmp,"zws_%d",i % 1000);  
        boost_v.push_back( boost::flyweight<string>( tmp )  );  
    }  
    sleep(10);
    return ;  
}

void WithoutFlyWeight()
{
    char tmp[10];  
    vector< string >  stl_v;  
    for(int i =0;i< 10000000;i++)  
    {  
        sprintf(tmp,"zws_%d",i % 1000);  
        stl_v.push_back( (tmp)  );  
    }  
    cout <<"========="<<endl;  
    sleep(10);
    return ;  
}

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        return -1;
    }
    int flag = atoi(argv[1]);
    if (flag == 0)
    {
        WithFlyWeight();
    }
    else
    {
        WithoutFlyWeight();
    }
    return 0;
}
