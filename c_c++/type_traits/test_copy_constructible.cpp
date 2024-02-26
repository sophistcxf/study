#include <string>
#include <iostream>
#include <type_traits>
 
struct S1
{
    std::string str; // member has a non-trivial copy constructor
};
static_assert(std::is_copy_constructible_v<S1> == true);
static_assert(std::is_trivially_copy_constructible_v<S1> == false);
 
struct S2
{
    int n;
    S2(const S2&) = default; // trivial and non-throwing
};
static_assert(std::is_copy_constructible_v<S2> == true);
static_assert(std::is_trivially_copy_constructible_v<S2> == true);
static_assert(std::is_nothrow_copy_constructible_v<S2> == true);

struct S3 {
    S3(const S3&) = delete;
};

struct S4 {
private:
    S4(const S4&);
};

void test1() {
    std::cout << std::boolalpha;
    std::cout<< std::is_copy_constructible_v<int> << std::endl; // true
    std::cout << std::is_copy_constructible_v<S3> << std::endl; // false
    std::cout << std::is_copy_constructible_v<S4> << std::endl; // false
}
 
int main() {
    test1();
}
