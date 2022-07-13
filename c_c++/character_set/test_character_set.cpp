/*************************************************************************
  > File Name: test_character_set.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 五  7/ 6 00:14:59 2018
 ************************************************************************/

#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *str = "123汉ABC";
    char *p;

    printf("%d |", strlen(str));
    for(p=str; *p; p++) {
        printf(" %.2X", (unsigned char)(*p));
    }
    printf(" | %s\n", str);

    return 0;
}
