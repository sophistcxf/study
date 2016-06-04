/*************************************************************************
  > File Name: test_set_new_handler.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sat 04 Jun 2016 03:00:31 PM CST
 ************************************************************************/

#include <iostream>     // std::cout
#include <cstdlib>      // std::exit
#include <new>          // std::set_new_handler
using namespace std;

void no_memory () {
  std::cout << "Failed to allocate memory!\n";
  std::exit (1);
}

int main () {
  std::set_new_handler(no_memory);
  std::cout << "Attempting to allocate 1 GiB...";
  char* p = new char [1024*1024*1024];
  std::cout << "Ok\n";
  delete[] p;
  return 0;
}
