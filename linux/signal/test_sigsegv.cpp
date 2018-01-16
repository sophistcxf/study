/*************************************************************************
  > File Name: test_sigsegv.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 日  1/ 7 09:43:18 2018
 ************************************************************************/

#include <iostream>
#include <signal.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;


class Tester
{
public:
    virtual int test(int argc, char* argv[]) = 0;
};

class Tester1 : public Tester
{
public:
    static void signal_handler(int sig)
    {
        printf("received signal %d !!!\n",sig);
    }
    virtual int test(int argc, char* argv[])
    {
        /****************** man page 建议使用 sigaction 替代 signal ***********************/
        /* signal(SIGSEGV, signal_handler1); */
        struct sigaction act;
        struct sigaction old_act;
        act.sa_handler = Tester1::signal_handler;
        sigaction(SIGSEGV, &act, &old_act);
        int* s = 0;
        (*s) = 1;
        return 0;
    }
};

//信号处理函数
int main(int argc,char** argv)
{
    Tester1 tester1;
    tester1.test(argc, argv);
    return 0;
}
