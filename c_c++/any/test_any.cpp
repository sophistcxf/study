#include <any>
#include <iostream>
#include <vector>
 
void test1()
{
    std::cout << std::boolalpha;
 
    // any type
    std::any a = 1;
    std::cout << a.type().name() << ": " << std::any_cast<int>(a) << '\n';
    a = 3.14;
    std::cout << a.type().name() << ": " << std::any_cast<double>(a) << '\n';
    a = true;
    std::cout << a.type().name() << ": " << std::any_cast<bool>(a) << '\n';
 
    // bad cast
    try
    {
        a = 1;
        // float的typeid为f，a.type()为i，所以会抛异常
        const std::type_info& ti1 = typeid(float);
        std::cout << ti1.name() << std::endl;
        std::cout << a.type().name() << std::endl;
        std::cout << std::any_cast<float>(a) << '\n';
    }
    catch (const std::bad_any_cast& e)
    {
        std::cout << e.what() << '\n';
    }
 
    // has value
    a = 2;
    if (a.has_value())
        std::cout << a.type().name() << ": " << std::any_cast<int>(a) << '\n';
 
    // reset
    a.reset();
    if (!a.has_value())
        std::cout << "no value\n";
 
    // pointer to contained data
    a = 3;
    int* i = std::any_cast<int>(&a);
    std::cout << *i << '\n';
}

void test2() {
    std::vector<std::any> v;
    v.push_back(1);
    v.push_back("hello");
    v.push_back(std::vector<int>());
    for (auto& e : v) {
        std::cout << e.type().name() << ",";
    }
    std::cout << "\n";
}

int main() {
    //test1();
    test2();
    return 0;
}
