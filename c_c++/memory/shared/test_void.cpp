#include <memory>
#include <iostream>

class A {
public:
    A() { std::cout << "A" << std::endl; }
    ~A() { std::cout << "~A" << std::endl; }
};

void test1() {
    {
        std::shared_ptr<void> p(new A);
    }
    std::cout << 2 << std::endl;
    {
        std::shared_ptr<A> p2(new A);
    }
    std::cout << 3 << std::endl;
    {
        // 可以通过 std::static_pointer_cast 将 shared_ptr<void> 转为 shared_ptr<T>
        std::shared_ptr<void> p(new A);
        //std::shared_ptr<A> p2 = p;
        std::shared_ptr<A> p2 = std::static_pointer_cast<A>(p);
        std::cout << "use_count: " << p2.use_count() << std::endl;
    }
    std::cout << 4 << std::endl;
    {
        // 如果 std::static_pointer_cast 转失败了，编译期会报错
        // std::shared_ptr<A> p(new A);
        // std::shared_ptr<int> p2 = std::static_pointer_cast<int>(p);
    }
}

int main() {
    test1();
    return 0;
}