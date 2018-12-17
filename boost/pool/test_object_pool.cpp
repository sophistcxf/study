/*************************************************************************
  > File Name: test_object_poo.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Wed 29 Jun 2016 09:51:45 AM CST
 ************************************************************************/

#include <iostream>
#include <vector>
using namespace std;

#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool_alloc.hpp>

int main()
{
  boost::object_pool<int> pool(32, 0);
  pool.construct();
  std::cout << pool.get_next_size() << '\n';
  pool.set_next_size(8);
  int* i = pool.malloc();
  *i = 10;
  int* i2 = pool.construct(20);
  cout << *i << "\t" << *i2 << endl;
  
  // test pool_allocator
  /*
  {
    time_t b = time(NULL);
    std::vector<int, boost::pool_allocator<int> > v;
    for (int i = 0; i < 100000000; ++i)
      v.push_back(i);
    v.clear();
    std::vector<int, boost::pool_allocator<int> > v2;
    for (int i = 0; i < 100000000; ++i)
      v2.push_back(i);
    v2.clear();
    boost::singleton_pool<boost::pool_allocator_tag, sizeof(int)>::release_memory();
    cout << "with pool_allocator: " << time(NULL) - b << endl;
  }
  {
    time_t b = time(NULL);
    std::vector<int> v;
    for (int i = 0; i < 100000000; ++i)
      v.push_back(i);
    v.clear();
    std::vector<int> v2;
    for (int i = 0; i < 100000000; ++i)
      v2.push_back(i);
    cout << "without pool_allocator: " << time(NULL) - b << endl;
  }
  */

  // test singleton_pool
  typedef boost::singleton_pool<int, sizeof(int)> singleton_int_pool;
  typedef boost::singleton_pool<int, sizeof(int)> singleton_int2_pool;
  int* s_i = static_cast<int*>(singleton_int_pool::malloc());
  *s_i = 10;
  singleton_int_pool::free(s_i);
  singleton_int_pool::release_memory();
  
  return 0;
}
