/*************************************************************************
  > File Name: infix2postfix.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 22 Jun 2016 02:16:40 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

template <class T>
class Stack
{
public:
  Stack() : _capacity(128)
  {
    _elements = NULL;
    if (capacity != 0) {
      _elements = new T[capacity];
      memset(_elements, 0, capacity);
    }
  } 
  char push(char c)
  {
    if (top == _capacity - 1) {
      _capacity *= 2;
      delete [] _elements;
      _elements = new char[_capacity];
    }
  }
private:
  size_t _capacity;
  size_t top;
  T* _elements;
};

int main()
{
  return 0;
}
