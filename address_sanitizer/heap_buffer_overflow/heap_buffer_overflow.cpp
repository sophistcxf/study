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

int main(int argc, char **argv) {
    test1();
}
