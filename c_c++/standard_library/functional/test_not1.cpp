// not1 example
#include <iostream>     // std::cout
#include <functional>   // std::not1
#include <algorithm>    // std::count_if

struct IsOdd {
  bool operator() (const int& x) const {return x%2==1;}
  typedef int argument_type;
};

int test1() {
  int values[] = {1,2,3,4,5};
  int cx = std::count_if (values, values+5, std::not1(IsOdd()));
  std::cout << "There are " << cx << " elements with even values.\n";
  return 0;
}

int main() {
    test1();
}