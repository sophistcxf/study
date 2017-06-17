/*************************************************************************
  > File Name: test_gethostbyname.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Thu Jul  7 22:41:07 2016
 ************************************************************************/

#include <iostream>
#include <cstdio>
using namespace std;

#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// print hostent's member
void print_hostent(const hostent* h)
{
  cout << "h_name: " << h->h_name << endl;
  cout << "h_addrtype: " << h->h_addrtype << endl;
  cout << "h_length: " << h->h_length << endl;
  
  for (size_t i = 0; h->h_aliases[i]; ++i)
    printf("aliases %d: %s\n", i, h->h_aliases[i]);
 
  for (size_t i = 0; h->h_addr_list[i]; ++i) {
    printf("h->h_addr_list %d: %s\n", i, h->h_addr_list[i]);
    const char* ip = inet_ntoa(*(struct in_addr*)h->h_addr_list[i]);
    printf("after inet_ntoa: %s\n", ip);
    printf("%d.%d.%d.%d\n", *(unsigned char*)h->h_addr_list[i], *(((unsigned char*)h->h_addr_list[i])+1), *(((unsigned char*)h->h_addr_list[i])+2), *(((unsigned char*)h->h_addr_list[i])+3));
  }
}

int main()
{
  // configure in /etc/hosts
  struct hostent* h = gethostbyname("cxf.fppub.com");
  print_hostent(h); 
  return 0;
}
