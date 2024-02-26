#include <iostream>
#include <memory>
#include <string>

template <typename T>
class MyAllocator {
public:
 // 类型别名
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    
    // 构造函数
    MyAllocator() = default;

    // 复制构造函数模板
    template <typename U>
    MyAllocator(const MyAllocator<U>&) {}

    // 分配内存
    T* allocate(std::size_t n) {
        if (n == 0) {
            return nullptr;
        }
        if (n > static_cast<std::size_t>(-1) / sizeof(T)) {
            throw std::bad_alloc();
        }
        void* p = ::operator new(n * sizeof(T));
        return static_cast<T*>(p);
    }

    // 释放内存
    void deallocate(T* p, std::size_t /* n */) {
        ::operator delete(p);
    }

    // 构造对象
    template <typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        new (p) U(std::forward<Args>(args)...);
    }

    // 析构对象
    template <typename U>
    void destroy(U* p) {
        p->~U();
    }
};

void test1() {
    std::allocator<int> alloc1;
    int* p = alloc1.allocate(10);
#if __cplusplus <= 202000L
    std::cout << alloc1.max_size() << std::endl;
#endif
}

void test2() {
    std::vector<int, std::allocator<int> > v;
    v.push_back(0);
    
    // static_assert((is_same<typename allocator_type::value_type, value_type>::value))
    // 说明模板参数需要是一致的
    // std::vector<int, std::allocator<double> > v1;

    // 使用自己的allocator
    std::vector<int, MyAllocator<int> > v2;
    v2.push_back(0);
}

int main()
{
    //test1();
    test2();
}