/*************************************************************************
  > File Name: test_define.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Mon 27 Jun 2016 12:33:08 PM CST
 ************************************************************************/

#include <stdio.h>
#include <iostream>
#include <sstream>
using namespace std;

// 在x两边加上双引号 
#define POUND_SIGN(x) (#x)

#ifdef WIN32
#define POUND_AT(x) (#@x)
#endif

string get_version()
{
#ifdef DEBUG
  return "debug_1.0.0";
#else
  return "release_1.0.0";
#endif
}

#define paster( n ) printf( "token"#n" = %d\n", token##n)

#ifdef TEST_ERROR
#error ERROR, DO NOT define TEST_ERROR
// 也就是说，碰到#error后会停车编译，但不会停车预处理
#error I will be printed as well.
#endif

#define identifier  \
do  \
{   \
    static std::ostringstream oss; \
    if (oss.str().empty())  \
    {   \
        oss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__;  \
    }   \
    std::cout << oss.str() << std::endl;  \
} while(false)

void f1()
{
    std::cout << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
    identifier;
    identifier;
}

int main()
{
	std::cout << __FUNCTION__ << std::endl;
	f1();
  cout << POUND_SIGN(hello world) << endl;
#ifdef WIN32
  char ch = POUND_AT(a);
#endif

  // predefined macros
  cout << __FILE__ << endl;
  cout << __cplusplus << endl;

  int token10 = 10;
  paster(10);
  return 0;
}
