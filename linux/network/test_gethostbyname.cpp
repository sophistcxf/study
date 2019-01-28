/*************************************************************************
  > File Name: test_gethostbyname.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 一  1/28 22:48:43 2019
 ************************************************************************/

#include <iostream>
using namespace std;

#include <netdb.h>
#include <arpa/inet.h>

extern int h_errno;

int main(int argc, char* argv[])
{
    struct hostent* hptr = gethostbyname(argv[1]);
    if (hptr == NULL)
    {
        std::cerr << "error code " << h_errno << ", msg " << hstrerror(h_errno) << std::endl;
        return 1;
    }
    std::cout << hptr->h_name << std::endl;
    for (char** p = hptr->h_aliases; *p != NULL; ++p)
    {
        std::cout << *p << "\t";
    }
    std::cout << std::endl;
    std::cout << hptr->h_addrtype << std::endl;
    std::cout << hptr->h_length << std::endl;
    switch (hptr->h_addrtype)
    {
    case AF_INET:
        char str[INET_ADDRSTRLEN];
        for (char** p = hptr->h_addr_list; *p != NULL; ++p)
        {
            std::cout << inet_ntop(hptr->h_addrtype, *p, str, sizeof(str)) << "\t";
        }
        std::cout << std::endl;
        break;
    default:
        std::cerr << "unknown address type" << std::endl;
        break;
    }
    return 0;
}
