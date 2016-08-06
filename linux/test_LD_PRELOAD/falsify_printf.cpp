/*************************************************************************
  > File Name: falsify_printf.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sat 06 Aug 2016 05:44:59 PM CST
 ************************************************************************/

#include <iostream>
#include <dlfcn.h>
using namespace std;

static int (*_printf)(const char *format, ...) = NULL;

int printf(const char *format, ...)
{
  if (_printf == NULL) {
    _printf = (int (*)(const char *format, ...))dlsym(RTLD_NEXT, "printf");
    __asm__ __volatile__ (
      "movl 0x4(%ebp), %eax \n"
      "addl $15, %eax \n"
      "movl %eax, 0x4(%ebp)"
    );
    return 1;
  }
  __asm__ __volatile__ (
    "addl $12, %%esp \n"
    "jmp *%0 \n"
            : /* no output registers */
            : "g" (_printf)
            : "%esp"
  );
}
