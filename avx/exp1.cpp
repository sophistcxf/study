#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <immintrin.h>  // AVX指令集头文件
#include <time.h>       // 计时

#define N 2048  // 矩阵维度 (必须是8的倍数)

// 普通矩阵乘法
void matmul_float(float* A, float* B, float* C) {
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            float a = A[i * N + k];
            for (int j = 0; j < N; j++) {
                C[i * N + j] += a * B[k * N + j];
            }
        }
    }
}

// AVX加速矩阵乘法
void matmul_avx(float* A, float* B, float* C) {
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            __m256 a = _mm256_set1_ps(A[i * N + k]);  // 广播标量值到整个向量
            
            // 每次处理8个元素（256位 / 32位 = 8）
            for (int j = 0; j < N; j += 8) {
                // 加载B和C的8个元素
                __m256 b = _mm256_load_ps(&B[k * N + j]);
                __m256 c = _mm256_load_ps(&C[i * N + j]);
                
                // 乘加运算：c = c + a * b
                c = _mm256_add_ps(c, _mm256_mul_ps(a, b));
                
                // 存回结果
                _mm256_store_ps(&C[i * N + j], c);
            }
        }
    }
}

// 初始化矩阵
void init_matrix(float* m) {
    for (int i = 0; i < N * N; i++) {
        m[i] = (float)rand() / RAND_MAX;
    }
}

// 验证结果一致性
int verify(float* ref, float* test) {
    for (int i = 0; i < N * N; i++) {
        if (fabs(ref[i] - test[i]) > 1e-3) {
            printf("验证失败 at %d: %.6f vs %.6f\n", i, ref[i], test[i]);
            return 0;
        }
    }
    return 1;
}

int main() {
    // 使用对齐内存分配（AVX需要32字节对齐）
    float* A = (float*)aligned_alloc(32, N * N * sizeof(float));
    float* B = (float*)aligned_alloc(32, N * N * sizeof(float));
    float* C_ref = (float*)aligned_alloc(32, N * N * sizeof(float));
    float* C_avx = (float*)aligned_alloc(32, N * N * sizeof(float));

    // 初始化矩阵
    srand(time(NULL));
    init_matrix(A);
    init_matrix(B);
    memset(C_ref, 0, N * N * sizeof(float));
    memset(C_avx, 0, N * N * sizeof(float));

    // 普通版本计时
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    matmul_float(A, B, C_ref);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_ref = (end.tv_sec - start.tv_sec) + 
                     (end.tv_nsec - start.tv_nsec) / 1e9;

    // AVX版本计时
    clock_gettime(CLOCK_MONOTONIC, &start);
    matmul_avx(A, B, C_avx);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_avx = (end.tv_sec - start.tv_sec) + 
                     (end.tv_nsec - start.tv_nsec) / 1e9;

    // 验证结果
    if (verify(C_ref, C_avx)) {
        printf("结果验证成功！\n");
        printf("普通版本: %.3f秒\n", time_ref);
        printf("AVX版本:  %.3f秒\n", time_avx);
        printf("加速比: %.2fx\n", time_ref / time_avx);
    }

    // 释放内存
    free(A);
    free(B);
    free(C_ref);
    free(C_avx);

    return 0;
}
