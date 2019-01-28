/*************************************************************************
  > File Name: test_getprotoent.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: 一  1/28 21:18:37 2019
 ************************************************************************/

#include <iostream>
using namespace std;

#include <netdb.h>

int main()
{
    while (true)
    {
        /* getprotoent 会打开 /etc/protocols */
        struct protoent* proto = getprotoent();
        if (proto == NULL)
            break;
        std::cout << proto->p_name << "\t" << proto->p_proto << std::endl;
    }
    return 0;
}
