/*************************************************************************
  > File Name: error.h
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sun 10 Jul 2016 12:15:03 AM PDT
 ************************************************************************/

#include <iostream>
#include <stdarg.h>
using namespace std;

void err_doit(int errnoflag, int level, const char* fmt, va_list ap);
void err_quit(const char* fmt, ...);
void err_sys(const char* fmt, ...);
