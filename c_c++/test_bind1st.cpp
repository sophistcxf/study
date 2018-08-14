/*************************************************************************
  > File Name: test_bind1st.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sun 12 Jun 2016 02:21:46 PM CST
 ************************************************************************/

#include <iostream>
#include <functional>
#include <algorithm>
using namespace std;


// bind1st's implementation
// change binary function object to unary function object 
template <class Operation> 
class my_binder1st : public std::unary_function<typename Operation::second_argument_type,
                                                typename Operation::result_type>
{
public:
  Operation op;
  // 记录其中一个参数
  typename Operation::first_argument_type value;
  // 构造函数中，用参数2对value赋值，用x对op赋值
  // op为二元functor, value为其一个参数
  my_binder1st(const Operation& x, const typename Operation::first_argument_type& v) : op(x), value(v) {}
  typename Operation::result_type operator() (const typename Operation::second_argument_type& x)
  {
    return op(value, x); 
  }
};

// different with standard implementation
template <class Operation>
my_binder1st<Operation> my_bind1st(const Operation& op, const typename Operation::second_argument_type& x)
{
    // 构造一个my_binder1st对象，构造函数的参数为op和x
  return my_binder1st<Operation>(op, x);
}

void test_mybind()
{
  int numbers[] = {10,20,30,40,50,10};
  int cx;
  cx = count_if (numbers, numbers+6, bind1st(equal_to<int>(),10) );
  cout << "There are " << cx << " elements that are equal to 10.\n";
  cx = count_if (numbers, numbers+6, bind1st(less<int>(), 30));
  cout << "there are " << cx << " elements that are bigger than 30.\n";
  cx = count_if (numbers, numbers+6, bind2nd(less<int>(), 30));
  cout << "there are " << cx << " elements that are less than 30.\n";

  cout << bind1st(std::plus<int>(), 10)(20) << endl;
  // my_bind1st生成了一个函数对象
  // 生成一个函数对象，其第1个参数固定了是20
  // std::plus<int>生成一个函数对象
  cout << my_bind1st(std::plus<int>(), 10)(20) << endl;
}

void test2()
{
    std::plus<int> p;
    std::cout << p(10, 15) << std::endl;
    // 使用bind1st生成functor, 绑定了一个参数
    std::binder1st<std::plus<int> > p2 = bind1st(std::plus<int>(), 10);
    int a = p2(30);
    std::cout << p2(30) << std::endl;
}

int main () 
{
    test2();
}
