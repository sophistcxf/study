#ifndef _JJ_ALLOCATOR_H_
#define _JJ_ALLOCATOR_H_

/*************************************************************************
  > File Name: jj_alloctor.h
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sat 04 Jun 2016 02:31:13 PM CST
 ************************************************************************/

/*
 * 此为《STL源码剖析》中实现的alloctor
 */

#include <iostream>
#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
using namespace std;

namespace JJ
{

template <class T>
inline T* _allocate(ptrdiff_t size, T*)
{
  set_new_handler(0);
  T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
  if (tmp == 0) {
    cerr << "out of memory" << endl;
  }
  return tmp;
}

template <class T>
inline void _deallocate(T* buffer)
{
  ::operator delete(buffer);
}

template <class T1, class T2>
inline void _construct(T1* p, const T2& value)
{
  new(p) T1(value);
}

template <class T>
inline void _destroy(T* ptr)
{
  ptr->~T();
}

template <class T>
class allocator
{
public:
  typedef T         value_type;
  typedef T*        pointer;
  typedef const T*  const_pointer;
  typedef T&        reference;
  typedef const T&  const_reference;
  typedef size_t    size_type;
  typedef ptrdiff_t difference_type;
  
  // rebind allocator of type U
  template <class U>
  struct rebind
  {
    typedef allocator<U> other;
  };

  pointer allocate(size_type n, const void* hint = 0)
  {
    return _allocate((difference_type)n, (pointer)0);
  }

  void deallocate(pointer p, size_type n) { _deallocate(p); }

  void construct(pointer p, const T& value) { _construct(p, value); }

  void destroy(pointer p) { _destroy(p); }

  pointer address(reference x) { return (pointer)&x; }

  const_pointer const_address(const_reference x) { return (const_pointer)&x; }

  size_type max_size() const { return size_type(UINT_MAX/sizeof(T)); }
};

} // namespace JJ

#endif /* _JJ_ALLOCATOR_H_ */
