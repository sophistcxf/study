#include <iostream>     // std::cout
#include <chrono>       // std::chrono::seconds, std::chrono::milliseconds
// std::chrono::duration_cast

void test1()
{
    using namespace std::chrono;
    // std::chrono::milliseconds is an instatiation of std::chrono::duration:
    std::chrono::milliseconds foo (1000); // 1 second
    foo*=60;

    std::cout << "duration (in periods): ";
    std::cout << foo.count() << " milliseconds.\n";

    std::cout << "duration (in seconds): ";
    std::cout << foo.count() * milliseconds::period::num / milliseconds::period::den;
    std::cout << " seconds.\n";

}

void test2()
{
    std::chrono::duration<int, std::ratio<3600,1> > hour;
}

int main ()
{
    test1();
    //test2();
    return 0;
}
