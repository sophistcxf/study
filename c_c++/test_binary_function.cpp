/*************************************************************************
  > File Name: test_binary_function.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Mon 20 Jun 2016 08:40:14 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include <functional>   // std::binary_function

struct Compare : public std::binary_function<int,int,bool> {
  bool operator() (int a, int b) {return (a==b);}
};

int main () {
  Compare Compare_object;
  Compare::first_argument_type input1;
  Compare::second_argument_type input2;
  Compare::result_type result;

  std::cout << "Please enter first number: ";
  std::cin >> input1;
  std::cout << "Please enter second number: ";
  std::cin >> input2;

  result = Compare_object (input1,input2);

  std::cout << "Numbers " << input1 << " and " << input2;
  if (result)
    std::cout << " are equal.\n";
  else
    std::cout << " are not equal.\n";

  return 0;
}
