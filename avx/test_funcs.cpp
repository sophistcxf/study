#include <immintrin.h>
#include <iostream>
#include <vector>

void test1() {
    float* a = (float*)aligned_alloc(32, 8 * sizeof(float));
    float* b = (float*)aligned_alloc(32, 8 * sizeof(float));
    for (int i = 0; i < 8; ++i) {
        a[i] = i;
        b[i] = i;
    }
    float* c = (float*)aligned_alloc(32, 8 * sizeof(float));
    
    __m256 a_avx = _mm256_load_ps(&a[0]);
    __m256 b_avx = _mm256_load_ps(&b[0]);
    __m256 c_avx = _mm256_mul_ps(a_avx, b_avx);
    _mm256_store_ps(&c[0], c_avx);
    for (int i = 0; i < 8; ++i) {
        std::cout << c[i] << ",";
    }
    std::cout << std::endl;
}

int main() {
    test1();
}
