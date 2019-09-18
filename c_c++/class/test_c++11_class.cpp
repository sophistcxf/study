#include <iostream>

class Person
{
public:
    /* c++11 可以 in-class initialization of non-static data member */
    std::string name = "undefined";
    int age = 10;
};

void test_construct()
{
    Person person;
    std::cout << person.name << "\t" << person.age << std::endl;
}

int main(int argc, char* argv[])
{
    test_construct();
}
