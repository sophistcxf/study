#include <iostream>

class MyString
{
public:
    MyString(int size) : size_(size) {}
public:
    int size() const { return size_; }
private:
    int size_;
};


class MyString2
{
public:
    MyString2(int size) : size_(size) {}
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

void print(const MyString2 str)
{
    std::cout << "print(const MyString2 str)" << std::endl;
    std::cout << str.size() << std::endl;
}

int main()
{
    // 调用会歧义，不知道调用哪个 print
    print(10);
    return 0;
}
