#include <iostream>

class MyString
{
public:
    explicit MyString(int size) : size_(size) {}
public:
    int size() const { return size_; }
private:
    int size_;
};


void print(const MyString str)
{
    std::cout << "print(const MyString str)" << std::endl;
    std::cout << str.size() << std::endl;
}

void print(int v)
{
    std::cout << "print(int v)" << std::endl;
    std::cout << v << std::endl;
}

int main()
{
    print(10);
    return 0;
}
