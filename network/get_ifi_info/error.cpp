/*************************************************************************
  > File Name: error.h
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sun 10 Jul 2016 12:04:39 AM PDT
 ************************************************************************/

#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>   // for LOG priorities
using namespace std;

#include <errno.h>

#define MAXLINE 1024

void err_doit(int errnoflag, int level, const char* fmt, va_list ap)
{
 
    int     errno_save, n;
    char    buf[MAXLINE + 1];
    errno_save = errno;     /* value caller might want printed */
#ifdef  HAVE_VSNPRINTF
    vsnprintf(buf, MAXLINE, fmt, ap);   /* safe */
#else
    vsprintf(buf, fmt, ap);                 /* not safe */
#endif
    n = strlen(buf);
    if (errnoflag)
        snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
    strcat(buf, "\n");

    fflush(stdout);     /* in case stdout and stderr are the same */
    fputs(buf, stderr);
    fflush(stderr);
    return; 
}

void err_quit(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  err_doit(0, LOG_ERR, fmt, ap);
  va_end(ap);
  exit(1);
}

void err_sys(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(1, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(1);
}

