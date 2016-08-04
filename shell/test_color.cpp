/*************************************************************************
  > File Name: test_color.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Thu 04 Aug 2016 07:24:01 PM CST
 ************************************************************************/

#include <iostream>
#include <cstdio>
using namespace std;

#define COLOR_NONE          "\033[m"  
#define COLOR_RED           "\033[0;32;31m"  
#define COLOR_LIGHT_RED     "\033[1;31m"  
#define COLOR_GREEN         "\033[0;32m"  
#define COLOR_LIGHT_GREEN   "\033[1;32m"  
#define COLOR_BLUE          "\033[0;32;34m"  
#define COLOR_LIGHT_BLUE    "\033[1;34m"  
#define COLOR_DARY_GRAY     "\033[1;30m"  
#define COLOR_CYAN          "\033[0;36m"  
#define COLOR_LIGHT_CYAN    "\033[1;36m"  
#define COLOR_PURPLE        "\033[0;35m"  
#define COLOR_LIGHT_PURPLE "\033[1;35m"  
#define COLOR_BROWN         "\033[0;33m"  
#define COLOR_YELLOW        "\033[1;33m"  
#define COLOR_LIGHT_GRAY    "\033[0;37m"  
#define COLOR_WHITE         "\033[1;37m"
#define COLOR_GREEN_BACK    "\033[42m"

#define BLINK               "\033[5m"
#define UNDERLINE           "\033[4m"

int main()
{
  printf("%s%sI'm red, blink\n", BLINK, COLOR_RED);
  printf("%sI'm green\n", COLOR_GREEN);
  printf("%s%sI have underline\n", COLOR_YELLOW, UNDERLINE);
  printf("%sI have green background\n", COLOR_GREEN_BACK);
  // set to default
  printf("\033[0m\n");
  return 0;
}
