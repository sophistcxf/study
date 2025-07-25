#include <iostream>
#include <iomanip>
#include <immintrin.h>
#include <cmath>
#include <ctime>

// 编译选项需要 -mavx2 -mfma (FMA版) 和 -mavx2 -mno-fma (非FMA版)
// clang++ -mavx2 -mfma -O3 -o fma_test fma_test.cpp

constexpr size_t ARRAY_SIZE = 1024 * 1024 * 128;  // 16M元素
constexpr size_t ALIGNMENT = 32;                // AVX对齐要求
constexpr int ITERATIONS = 10;                  // 测试迭代次数

// 使用FMA指令的版本
void fmadd_avx(const float* a, const float* b, const float* c, float* result) {
    for (size_t i = 0; i < ARRAY_SIZE; i += 8) {
        __m256 va = _mm256_load_ps(&a[i]);
        __m256 vb = _mm256_load_ps(&b[i]);
        __m256 vc = _mm256_load_ps(&c[i]);
        __m256 res = _mm256_fmadd_ps(va, vb, vc); // a*b + c
        _mm256_store_ps(&result[i], res);
    }
}

// 不使用FMA的普通版本
void mul_add_avx(const float* a, const float* b, const float* c, float* result) {
    for (size_t i = 0; i < ARRAY_SIZE; i += 8) {
        __m256 va = _mm256_load_ps(&a[i]);
        __m256 vb = _mm256_load_ps(&b[i]);
        __m256 vc = _mm256_load_ps(&c[i]);
        __m256 res = _mm256_add_ps(_mm256_mul_ps(va, vb), vc); // (a*b) + c
        _mm256_store_ps(&result[i], res);
    }
}

// 验证结果一致性
bool verify(const float* ref, const float* test) {
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        if (fabs(ref[i] - test[i]) > 1e-5) {
            std::cerr << std::setprecision(10);
            std::cerr << "验证失败 at " << i 
                      << ": " << ref[i] << " vs " << test[i] << std::endl;
            return false;
        }
    }
    return true;
}

int main() {
    // 分配对齐内存
    float* a = static_cast<float*>(aligned_alloc(ALIGNMENT, ARRAY_SIZE * sizeof(float)));
    float* b = static_cast<float*>(aligned_alloc(ALIGNMENT, ARRAY_SIZE * sizeof(float)));
    float* c = static_cast<float*>(aligned_alloc(ALIGNMENT, ARRAY_SIZE * sizeof(float)));
    float* res_fma = static_cast<float*>(aligned_alloc(ALIGNMENT, ARRAY_SIZE * sizeof(float)));
    float* res_normal = static_cast<float*>(aligned_alloc(ALIGNMENT, ARRAY_SIZE * sizeof(float)));

    // 初始化数据
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        a[i] = static_cast<float>(i % 100) * 0.1f;
        b[i] = static_cast<float>(i % 50) * 0.2f;
        c[i] = static_cast<float>(i % 200) * 0.05f;
    }

    // 执行计算
    clock_t start = clock();
    for (int i = 0; i < ITERATIONS; ++i) {
        fmadd_avx(a, b, c, res_fma);
    }
    double fma_time = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;

    start = clock();
    for (int i = 0; i < ITERATIONS; ++i) {
        mul_add_avx(a, b, c, res_normal);
    }
    double normal_time = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;

    // 验证结果
    bool valid = verify(res_fma, res_normal);

    // 输出结果
    std::cout << "========= 性能对比 =========\n"
              << "FMA版本平均耗时:   " << fma_time/ITERATIONS << " 秒/次\n"
              << "普通版本平均耗时:  " << normal_time/ITERATIONS << " 秒/次\n"
              << "加速比:          " << normal_time / fma_time << "x\n"
              << "结果验证:        " << (valid ? "通过" : "失败") 
              << "\n============================\n";

    // 释放内存
    free(a);
    free(b);
    free(c);
    free(res_fma);
    free(res_normal);

    return valid ? 0 : 1;
}
