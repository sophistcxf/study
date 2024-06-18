#include <iostream>
#include <memory>
#include <cstdlib>

// 如果space不够
void test1() {
    size_t size = 1024;
    void* buffer = std::malloc(size + 64);

    std::cout << "原始地址: " << buffer << std::endl;
    size_t alignment = 256;

    std::cout << (long)buffer % 256 << std::endl;

    size_t space = size;
    void* aligned_address = buffer;

    if (std::align(alignment, size, aligned_address, space)) {
        std::cout << "对齐地址: " << aligned_address << std::endl;
    } else {
        std::cerr << "无法对齐内存" << std::endl;
    }
    std::free(buffer);
}

void test2() {
    // 分配一块足够大的内存。
    size_t size = 1024; // 我们需要的内存大小。
    void* buffer = std::malloc(size + 64); // 分配额外的内存以确保对齐的空间。

    std::cout << "原始地址: " << buffer << std::endl;

    // 需要对齐的大小，例如对齐到32字节边界。
    size_t alignment = 32;

    // std::align 需要的辅助变量。
    size_t space = size;
    void* aligned_address = buffer;


    if (std::align(alignment, size, aligned_address, space)) {
        std::cout << "对齐地址: " << aligned_address << std::endl;

        // 在需要对齐的地址上进行操作，例如加载 SIMD 数据或传递到要求对齐内存的API。
    } else {
        std::cerr << "无法对齐内存" << std::endl;
    }

    // 释放内存。
    std::free(buffer);
}

int main() {
    test1();
    return 0;
}