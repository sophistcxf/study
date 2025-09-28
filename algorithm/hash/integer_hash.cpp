/*************************************************************************
  > File Name: integer_hash.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sun 03 Jul 2016 12:39:29 AM PDT
 ************************************************************************/

#include <iostream>
using namespace std;

template <class T>
class IntegerHash
{
public:
  IntegerHash(size_t capacity = 256) : _capacity(capacity)
  {
    _bucket = new T[capacity];
  }
  T* lookupOrInsert(int key)
  {
    int hash = (key * 2654436761ULL) % _capacity;
    for (int i = 0; i < _capacity; ++i) {
      int new_hash_code = (hash + i) % 
    }
  }
private:
  T* _bucket;
  size_t _capacity;
};

int main()
{
  return 0;
}
