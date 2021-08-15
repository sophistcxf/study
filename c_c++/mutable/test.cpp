#include <iostream>

class C
{
public:
    C() : i(0), i2(0) {}
    void add(int j) const
    {
        i += j;
    }
    void add2(int j) const
    {
        //i2 += j;    /* 编译失败 */
    }
    void add3(int j) {
        i2 += j;
    }
    void add4(int j) {
        i += j;
    }
    mutable int i;
    int i2;
};

int main(int argc, char* argv[])
{
    C c;
    c.i = 10;
    std::cout << c.i << std::endl;
    c.add(2);
    std::cout << c.i << std::endl;
    const C c1;
    c1.i = 10;
    //c1.add3(2);
    //c1.add4(2); // 编译失败，const的对象，只能调用const修饰的函数，哪怕 i 是 mutable的
}
