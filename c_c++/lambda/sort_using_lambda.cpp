#include <iostream>
#include <algorithm>
using namespace std;
int main()
{
    int a[5] = { 2, 1, 5, 4, 3 };
    int total = 0;
    for_each(a, a + 5, [&](int & x) { total += x; x *= 2; });
    cout << total << endl;  //输出 10
    for_each(a, a + 5, [=](int x) { cout << x << " "; });
    std::sort(a, a+5, [=](int x, int y) -> bool {return x < y; } );
    std::cout << std::endl;
    for_each(a, a + 5, [=](int x) { cout << x << " "; });
    return 0;
}
