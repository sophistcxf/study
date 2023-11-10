#include <iostream>     // std::cout
#include <functional>   // std::reference_wrapper
                        //
namespace spf = std::placeholders;

class A {
public:
    virtual void foo() = 0;
};

class B : public A {
public:
    virtual void foo() {}
};

void test1() {
  int a(10),b(20),c(30);

  // an array of "references":
  std::reference_wrapper<int> refs[] = {a,b,c};

  std::cout << "refs:";
  for (int& x : refs) {
      std::cout << ' ' << x;
  }
  std::cout << '\n';

  // 这是无效的，reference_wrapper一定要被初始化
  //std::reference_wrapper<int> r;
}

std::reference_wrapper<int> foo() {
    int n = 10;
    return n;
}

int add(int a, int b) {
    return a + b;
}

void test2() {
    auto n = foo();
    std::cout << n << std::endl;
    std::cout << n.get() << std::endl;

    int value = 10;
    std::reference_wrapper<int> r = value;
    std::cout << r << "," << r.get() << '\n';
    // &value与&(r.get())是相等的，说明引用同一个对象
    std::cout << &value << "," << &r << "," << &(r.get()) << '\n';
    r.get() = 100;
    std::cout << r << '\n';
    auto cr = std::cref(r);
    // cr.get() 返回的是const value，不能修改
    //cr.get() = 1000;

    auto add_fun = std::bind(add, spf::_1, spf::_2);
    auto ref_fun = std::ref(add_fun);
    std::cout << ref_fun(10, 20) << std::endl;
}

void test3() {
    std::reference_wrapper<int*> n((int*)nullptr);
}

int main() {
    test3();
}
