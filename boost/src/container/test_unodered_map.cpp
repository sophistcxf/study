/*************************************************************************
  > File Name: test_unodered_map.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 2016年11月04日 星期五 14时44分46秒
 ************************************************************************/

#include <map>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

void test_1();
void test_2();

int main()
{
    typedef boost::unordered_map<int, std::vector<int> > unordered_map;
    unordered_map m;
    for (int i = 0; i < 10; ++i)
      m.insert(make_pair(i, std::vector<int>()));
    m.insert(make_pair(9, std::vector<int>()));
    for (unordered_map::iterator it = m.begin(); it != m.end(); ++it)
      cout << it->first << endl;
}

void test_1()
{
  typedef boost::unordered_map<std::string, int> unordered_map;
  unordered_map map;

  map.emplace("cat", 4);
  map.emplace("shark", 0);
  map.emplace("spider", 8);
  map.insert(std::make_pair("zhangsan", 10));

  for (unordered_map::iterator iter = map.begin(); iter != map.end(); ++iter)
    std::cout << iter->first << ";" << iter->second << '\n';

  std::cout << map.size() << '\n';
  std::cout << map.max_size() << '\n';

  std::cout << std::boolalpha << (map.find("cat") != map.end()) << '\n';
  std::cout << map.count("shark") << '\n';
}

void test_2()
{
  boost::unordered_set<unsigned> m;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < 100000; ++i)
  {
    m.emplace(i);
    v.push_back(i);
  }
  cout << "begin" << endl;
  time_t b = time(NULL);
  for (unsigned i = 0; i < 1000000; ++i)
  {
    cout << "eee";
    m.find(i);
  }
  cout << time(NULL) - b << endl;
  b = time(NULL);
  for (unsigned i = 0; i < 1000000; ++i)
  {
    find(v.begin(), v.end(), i);
  }
  cout << time(NULL) - b << endl;
}
