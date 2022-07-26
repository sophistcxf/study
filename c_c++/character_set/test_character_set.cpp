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

void test1() {
    char *str = "123汉ABC";
    char *p;

    printf("%d |", strlen(str));
    for(p=str; *p; p++) {
        printf(" %.2X", (unsigned char)(*p));
    }
    printf(" | %s", str);

    std::cout << std::endl;
    uint16_t* s = (uint16_t*)"123汉ABC";
    for(p=(char*)s; *p; p++) {
        printf(" %.2X", (unsigned char)(*p));
    }


    std::cout << std::endl;
    uint16_t* s2 = (uint16_t*)L"123汉ABC";
    for(p=(char*)s2; *p; p++) {
        printf(" %.2X", (unsigned char)(*p));
    }
    std::cout << std::endl;
}

void test2() {
    wchar_t c = 'a';
    std::cout << c << std::endl;
    char c2 = 'a';
    std::cout << c2 << std::endl;
}

int main(int argc, char *argv[]) {
    //test1();
    test2();
}
