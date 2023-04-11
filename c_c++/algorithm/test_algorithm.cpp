#include <algorithm>
#include <iostream>
#include <array>
#include <vector>

void test_if_heap()
{
    std::vector<int> foo{9, 5, 2, 6, 4, 1, 3, 8, 7};

    if (!std::is_heap(foo.begin(), foo.end())) {
        std::cout << "foo is not heap" << std::endl;
        std::make_heap(foo.begin(), foo.end());
    }

    std::cout << "Popping out elements:";
    while (!foo.empty())
    {
        std::pop_heap(foo.begin(), foo.end()); // moves largest element to back
        std::cout << ' ' << foo.back();        // prints back
        foo.pop_back();                        // pops element out of container
    }
    std::cout << '\n';
}

void test_all_of()
{
    std::array<int, 8> foo = {3, 5, 7, 11, 13, 17, 19, 23};
    if (std::all_of(foo.begin(), foo.end(), [](int i)
                    { return i % 2; }))
    {
        std::cout << "All the elements are odd numbers.\n";
    }

    if (std::any_of(foo.begin(), foo.end(), [](int i)
                    { return i == 13; }))
    {
        std::cout << "There are negative elements in the range.\n";
    }
}

int main()
{
    //test_all_of();
    test_if_heap();
    return 0;
}