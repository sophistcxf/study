#include "exp1.h"

int g_year = 2024;

class C {
public:
    C() {}
    void foo() {
        int value = 10;
    }
    void foo3() {
        foo();
    }
    static void foo1() {

    }
private:
    const char* name = "zhangsan";
    int age = 20;
    static bool is_young;
};

bool C::is_young = true;


template <class T>
T add(const T&& t1, const T&& t2) {
    return t1 + t2;
}

bool isMunicipality(City city) {
    return city == kBeijing || city == kShanghai;
}

int main(int argc, char* argv[]) {
    return 0;
}
