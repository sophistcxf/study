#include <iostream>

class C
{
public:
    void add(int j) const
    {
        i += j;
    }
    void add2(int j) const
    {
        //i2 += j;    /* 编译失败 */
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
}
