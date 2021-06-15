// RUN: clang -O -g -fsanitize=address %t && ./a.out
//
#include <iostream>

void test1()
{
    int *array = new int[100];
    array[0] = 0;
    int res = array[110];  // BOOM
    delete [] array;
    std::cout << res << std::endl;
}

void test2()
{
    int a = 10;
    double* p = new double[10];
    std::cout << p[-1] << std::endl;
}

int main(int argc, char **argv) {
    test2();
}
