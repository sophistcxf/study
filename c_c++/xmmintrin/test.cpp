/*************************************************************************
  > File Name: test.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 01 Aug 2018 10:26:02 PM CST
 ************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <xmmintrin.h>
#include <string.h>
#include <time.h>
using namespace std;

int main(int argc, char* argv[])
{
    int n_vals = 1024 * 1024 * 1024;
    float* vals = new float[n_vals];
    memset(vals, 0, sizeof(float) * n_vals);
    float a = 2.0, b = 3.0;
    time_t beg = time(NULL);
    for (int i = 0; i < n_vals; ++i)
    {
        vals[i] = vals[i] * a + b;
    }
    time_t e = time(NULL);
    cout << e - beg << endl;
    beg = time(NULL);
    for (int i = 0; i < n_vals; i+=4)
    {
        vals[i+0] = vals[i+0] * a + b;
        vals[i+1] = vals[i+1] * a + b;
        vals[i+2] = vals[i+2] * a + b;
        vals[i+3] = vals[i+3] * a + b;
    }
    e = time(NULL);
    cout << e - beg << endl;
    float* buf_aligned = NULL;
    posix_memalign((void**)&buf_aligned, 16, n_vals*sizeof(float));
    memset(buf_aligned, 0, sizeof(float) * n_vals);
    beg = time(NULL);
    for (int i = 0; i < n_vals; i += 4)
    {
        __m128 sse_a = _mm_load1_ps(&a);
        __m128 sse_b = _mm_load1_ps(&b);
        __m128 v = _mm_load_ps(&buf_aligned[i]);
        v=_mm_add_ps(_mm_mul_ps(v, sse_a), sse_b);
        _mm_store_ps(&buf_aligned[i], v);
    }
    e = time(NULL);
    cout << e - beg << endl;
    beg = e;
    return 0;
}
