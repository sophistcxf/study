#include <iostream>
#include <functional>
using namespace std;

int main(int argc, char* argv[])
{
    std::function<void(int)> f1;
    void (*f1_target)(int) = f1.target<void(*)(int)>();
    if (f1_target == NULL) { std::cout << "f1_target == NULL" << std::endl; }
    return 0;
}
