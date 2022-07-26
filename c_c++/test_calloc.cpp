/*************************************************************************
  > File Name: test_calloc.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Tue 19 Jul 2016 09:26:07 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

/* calloc example */
#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* calloc, exit, free */

void test1() {
  // calloc set memory to 0
  int* p = (int*)calloc(10, sizeof(int)); 
  for (int i = 0; i < 10; ++i)
    printf("%d\t", p[i]);
  printf("\n");
  free (p);
  p = (int*)malloc(sizeof(int)*100);
  for (int i = 0; i < 100; ++i)
    printf("%d\t", p[i]);
  printf("\n");
}

void test2() {
    char* sz = (char*)malloc(0);
    *sz = 'a';
    std::cout << sz << std::endl;
}

int main () {
    //test1();
    test2();
}
