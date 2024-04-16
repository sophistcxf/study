#include <iostream>
#include <type_traits>

class C {
public:
    C() {}
private:
    C(const C& other) {

    }
};

void test1() {
    // C是非copy constructible的
    std::cout << std::is_copy_constructible<C>() << std::endl;
    std::vector<C> v;
    C c;
    //v.push_back(c);   // 非copy constructible的类，不能push_back给vector
}

int main() {
    test1();
    return 0;
}
