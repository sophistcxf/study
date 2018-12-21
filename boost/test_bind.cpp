/*************************************************************************
  > File Name: test_bind.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sun 12 Jun 2016 10:31:30 AM CST
 ************************************************************************/

#include <iostream>
using namespace std;
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include "boost/lambda/lambda.hpp"
#include "boost/lambda/bind.hpp"

int f(int a, int b) { return a + b; }
int g(int a, int b, int c) { return a + b * c; }
typedef int (*f_type)(int a, int b);
typedef int (*g_type)(int a, int b, int c);

int main()
{
  using namespace boost::lambda;

  // bind simple function
  cout << bind(f, 1, 2)() << endl;
  cout << bind(g, 1, 2, 3)() << endl;

  // placeholders
  cout << bind(f, _1, 2)(1) << endl;
  // cout << bind(f, _2, 2)(1) << endl;  // error
  cout << bind(f, _2, 2)(1, 2) << endl;
  cout << bind(g, _3, _2, _2)(1,2,3) << endl;

  typedef std::map<int,std::string> type;
  type keys_and_values;
  keys_and_values[3]="Less than pi";
  keys_and_values[42]="You tell me";
  keys_and_values[0]="Nothing, if you ask me";
  std::cout << "What's wrong with the following expression?\n";
  std::for_each( keys_and_values.begin(), keys_and_values.end(),
    std::cout << "key=" << bind(&type::value_type::first,_1) << ", value=" << bind(&type::value_type::second,_1) << '\n'); 
  std::cout << "\n...and why does this work as expected?\n";
  std::for_each( keys_and_values.begin(), keys_and_values.end(),
    std::cout << constant("key=") << bind(&type::value_type::first,_1) << ", value=" << bind(&type::value_type::second,_1) << '\n'); 
  std::cout << '\n'; 
  "keys_and_values.size()=" << bind(&type::size,_1) << "\nkeys_and_values.max_size()=" << bind(&type::max_size,_1)(keys_and_values);
  return 0;
}
