#include <immintrin.h>
#include <iostream>

void print_results(const char* title, const int16_t* data, size_t count) {
    std::cout << title << ": ";
    for (size_t i = 0; i < count; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << "\n";
}

int main() {
    // 输入数据：无符号字节（ymm2）和有符号字节（ymm3）
    alignas(32) uint8_t a[32] = {1, 2, 3, 4, 5, 6, 7, 8, 
                                9, 10, 11, 12, 13, 14, 15, 16,
                                17, 18, 19, 20, 21, 22, 23, 24,
                                25, 26, 27, 28, 29, 30, 31, 32};
    alignas(32) int8_t b[32] = {-1, -2, -3, -4, 5, 6, 7, 8,
                               9, 10, 11, 12, 13, 14, 15, 16,
                               17, 18, 19, 20, 21, 22, 23, 24,
                               25, 26, 27, 28, 29, 30, 31, 32};

    __m256i ymm_a = _mm256_load_si256(reinterpret_cast<const __m256i*>(a));
    __m256i ymm_b = _mm256_load_si256(reinterpret_cast<const __m256i*>(b));

    // 执行 VPMADDUBW
    __m256i ymm_result = _mm256_maddubs_epi16(ymm_a, ymm_b);

    // 提取结果
    alignas(32) int16_t result[16];
    _mm256_store_si256(reinterpret_cast<__m256i*>(result), ymm_result);

    print_results("计算结果", result, 16);
    return 0;
}
