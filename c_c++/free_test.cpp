#include <iostream>
#include <stdio.h>
#include <stdlib.h>

class C
{
public:
    int n;
};

class C2
{
public:
    int n;
    double d;
};

int main()
{

    std::cout << sizeof(C) << std::endl;
    std::cout << sizeof(C2) << std::endl;

    int a = 10;
    int b = 20;

    C* pc = new C;
    pc->n = 10;


    C* pc2 = (C*)malloc(sizeof(C));
    pc2->n = 10;

    std::cout << a << b << std::endl;

    return 0;
}
