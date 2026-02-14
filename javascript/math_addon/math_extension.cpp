#include <napi.h>
#include <cmath>
#include <vector>

// 数学计算函数
double calculate_factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    double result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

double calculate_fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    
    double prev = 0, curr = 1;
    for (int i = 2; i <= n; i++) {
        double temp = curr;
        curr = prev + curr;
        prev = temp;
    }
    return curr;
}

// 同步阶乘函数
Napi::Number FactorialSync(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
        return Napi::Number::New(env, -1);
    }
    
    int n = info[0].As<Napi::Number>().Int32Value();
    double result = calculate_factorial(n);
    return Napi::Number::New(env, result);
}

// 同步斐波那契函数
Napi::Number FibonacciSync(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
        return Napi::Number::New(env, -1);
    }
    
    int n = info[0].As<Napi::Number>().Int32Value();
    double result = calculate_fibonacci(n);
    return Napi::Number::New(env, result);
}

// 数组求和函数
Napi::Number ArraySum(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !info[0].IsArray()) {
        Napi::TypeError::New(env, "Array expected").ThrowAsJavaScriptException();
        return Napi::Number::New(env, 0);
    }
    
    Napi::Array arr = info[0].As<Napi::Array>();
    double sum = 0;
    
    for (uint32_t i = 0; i < arr.Length(); i++) {
        Napi::Value val = arr[i];
        if (val.IsNumber()) {
            sum += val.As<Napi::Number>().DoubleValue();
        }
    }
    
    return Napi::Number::New(env, sum);
}

// 矩阵乘法函数 - 修复类型转换问题
Napi::Array MatrixMultiply(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsArray() || !info[1].IsArray()) {
        Napi::TypeError::New(env, "Two arrays expected").ThrowAsJavaScriptException();
        return Napi::Array::New(env);
    }
    
    Napi::Array matrixA = info[0].As<Napi::Array>();
    Napi::Array matrixB = info[1].As<Napi::Array>();
    
    // 获取矩阵维度
    uint32_t rowsA = matrixA.Length();
    if (rowsA == 0) return Napi::Array::New(env);
    
    // 正确获取第一行来确定列数
    Napi::Value firstRowVal = matrixA[static_cast<uint32_t>(0)];
    if (!firstRowVal.IsArray()) return Napi::Array::New(env);
    Napi::Array firstRow = firstRowVal.As<Napi::Array>();
    uint32_t colsA = firstRow.Length();
    
    // 获取矩阵B的第一行来确定列数
    Napi::Value firstRowBVal = matrixB[static_cast<uint32_t>(0)];
    if (!firstRowBVal.IsArray()) return Napi::Array::New(env);
    Napi::Array firstRowB = firstRowBVal.As<Napi::Array>();
    uint32_t colsB = firstRowB.Length();
    
    uint32_t rowsB = matrixB.Length();
    
    // 检查矩阵维度是否匹配
    if (colsA != rowsB) {
        Napi::TypeError::New(env, "Matrix dimensions don't match for multiplication").ThrowAsJavaScriptException();
        return Napi::Array::New(env);
    }
    
    // 创建结果矩阵
    Napi::Array result = Napi::Array::New(env, rowsA);
    for (uint32_t i = 0; i < rowsA; i++) {
        Napi::Array row = Napi::Array::New(env, colsB);
        for (uint32_t j = 0; j < colsB; j++) {
            double sum = 0;
            for (uint32_t k = 0; k < colsA; k++) {
                // 正确访问矩阵元素
                Napi::Value rowAVal = matrixA[i];
                Napi::Value rowBVal = matrixB[k];
                
                if (rowAVal.IsArray() && rowBVal.IsArray()) {
                    Napi::Array rowA = rowAVal.As<Napi::Array>();
                    Napi::Array rowB = rowBVal.As<Napi::Array>();
                    
                    Napi::Value elemAVal = rowA[k];
                    Napi::Value elemBVal = rowB[j];
                    
                    if (elemAVal.IsNumber() && elemBVal.IsNumber()) {
                        double a = elemAVal.As<Napi::Number>().DoubleValue();
                        double b = elemBVal.As<Napi::Number>().DoubleValue();
                        sum += a * b;
                    }
                }
            }
            row[j] = Napi::Number::New(env, sum);
        }
        result[i] = row;
    }
    
    return result;
}

// 初始化模块
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("factorialSync", Napi::Function::New(env, FactorialSync));
    exports.Set("fibonacciSync", Napi::Function::New(env, FibonacciSync));
    exports.Set("arraySum", Napi::Function::New(env, ArraySum));
    exports.Set("matrixMultiply", Napi::Function::New(env, MatrixMultiply));
    return exports;
}

NODE_API_MODULE(math_extension, Init)