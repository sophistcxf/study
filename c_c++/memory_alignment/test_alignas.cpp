/*************************************************************************
  > File Name: test_alignas.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 五  1/15 22:27:15 2021
 ************************************************************************/

#include <iostream>

class A
{
    int i;
    char a;
};

class alignas(16) B
{
    int i;
    char a;
};

void test1()
{
    std::cout << sizeof(A) << std::endl;
    std::cout << sizeof(B) << std::endl;
}

void test2() {
    struct alignas(float) struct_float
    {
        // your definition here
    };

    struct alignas(A) struct_A {
    };

    // sse_t按32字节对齐，所以它的每个对象是32个字节
    struct alignas(32) sse_t
    {
        float sse_data[4];
    };

    /**
     * requested alignment is less than minimum alignment of 4 for type
     */
    struct alignas(1) struct_a1 {
        char a;
        int i;
    };

    // the array "cacheline" will be aligned to 64-byte boundary:
    alignas(64) char cacheline[64];


    struct default_aligned { float data[4]; } a, b, c;
    sse_t x, y, z;

    std::cout
        << "alignof(struct_float) = " << alignof(struct_float) << '\n'
        << "sizeof(sse_t) = " << sizeof(sse_t) << '\n'
        << "alignof(sse_t) = " << alignof(sse_t) << '\n'
        << "alignof(A) = " << alignof(A) << '\n'
        << "sizeof(A) = " << sizeof(A) << '\n'
        << "alignof(struct_A) = " << alignof(struct_A) << "\n"
        << "sizeof(struct_A) = " << sizeof(struct_A) << "\n"
        << "alignof(struct_a1) = " << alignof(struct_a1) << '\n'
        << "sizeof(struct_a1) = " << sizeof(struct_a1) << '\n'
        //<< "alignof(cacheline) = " << alignof(alignas(64) char[64]) << '\n'
        << std::hex << std::showbase
        << "&a: " << &a << '\n'
        << "&b: " << &b << '\n'
        << "&c: " << &c << '\n'
        << "&x: " << &x << '\n'
        << "&y: " << &y << '\n'
        << "&z: " << &z << '\n';
}

int main()
{
    test2();
    return 0;
}
