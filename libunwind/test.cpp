/*************************************************************************
  > File Name: test.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Tue 02 Aug 2016 08:23:25 PM CST
 ************************************************************************/

#define UNW_LOCAL_ONLY

#include <iostream>
#include <cstdio>
using namespace std;

#include "libunwind.h"

void show_backtrace (void) {
  unw_cursor_t cursor; unw_context_t uc;
  unw_word_t ip, sp;

  unw_getcontext(&uc);
  unw_init_local(&cursor, &uc);
  while (unw_step(&cursor) > 0) {
    unw_get_reg(&cursor, UNW_REG_IP, &ip);
    unw_get_reg(&cursor, UNW_REG_SP, &sp);
    printf ("ip = %lx, sp = %lx\n", (long) ip, (long) sp);
  }
}

int main()
{
  show_backtrace();
  return 0;
}
