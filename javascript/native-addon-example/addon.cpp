#include <napi.h>
#include <thread>   // 添加头文件
#include <chrono>   // 添加头文件

// 原生 C++ 函数
std::string greet(const std::string& name) {
    return "Hello, " + name + " from C++!";
}

// 异步工作线程
struct AsyncWorker : Napi::AsyncWorker {
    Napi::Promise::Deferred deferred;
    std::string name;
    std::string result;

    AsyncWorker(Napi::Env env, const std::string& name) 
        : Napi::AsyncWorker(env), deferred(Napi::Promise::Deferred::New(env)), name(name) {}

    void Execute() override {
        // 模拟耗时操作
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 现在应该可以编译通过
        result = greet(name);
    }

    void OnOK() override {
        deferred.Resolve(Napi::String::New(Env(), result));
    }

    void OnError(const Napi::Error& err) override {
        deferred.Reject(err.Value());
    }
};

// 同步方法
Napi::String GreetSync(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return Napi::String::New(env, "");
    }
    
    std::string name = info[0].As<Napi::String>();
    return Napi::String::New(env, greet(name));
}

// 异步方法 - 修复错误处理
Napi::Promise GreetAsync(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // 创建 Deferred 对象
    Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);
    
    if (info.Length() < 1 || !info[0].IsString()) {
        // 使用 Deferred 对象拒绝 Promise
        deferred.Reject(Napi::String::New(env, "Invalid argument"));
        return deferred.Promise();
    }
    
    std::string name = info[0].As<Napi::String>();
    AsyncWorker* worker = new AsyncWorker(env, name);
    worker->deferred = deferred;  // 使用传入的 deferred
    worker->Queue();
    return deferred.Promise();
}

// 初始化模块
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("greetSync", Napi::Function::New(env, GreetSync));
    exports.Set("greetAsync", Napi::Function::New(env, GreetAsync));
    return exports;
}

NODE_API_MODULE(addon, Init)
