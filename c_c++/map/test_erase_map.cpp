/*************************************************************************
  > File Name: test_erase_map.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Thu 04 Aug 2016 08:30:18 PM CST
 ************************************************************************/

#include <iostream>
#include <map>
using namespace std;

int main(int argc, char* argv[])
{
  map<int, int> m;
  for (int i = 0; i < 10; ++i)
    m.insert(make_pair(i, i));
  cout << "origin\n";
  for (map<int, int>::iterator iter = m.begin(); iter != m.end(); ++iter){
    cout << iter->first << endl;
  }
  for (map<int, int>::iterator iter = m.begin(); iter != m.end();){
    if (iter->second == 1 || iter->second == 5 || iter->second == 6)
      m.erase(iter++);
    else
      iter++;
  }
  cout << "after\n";
  for (map<int, int>::iterator iter = m.begin(); iter != m.end(); ++iter){
    cout << iter->first << endl;
  }
  return 0;
}
