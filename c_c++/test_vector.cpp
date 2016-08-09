/*************************************************************************
  > File Name: test_vector.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Fri 05 Aug 2016 05:57:06 PM CST
 ************************************************************************/

#include <iostream>
#include <vector>
using namespace std;


void test_erase()
{
  vector<int> v;
  for (int i = 0; i < 10; ++i) v.push_back(i);

  for (vector<int>::iterator iter = v.begin(); iter != v.end(); ) {
    if (*iter == 2 || *iter == 5 || *iter == 9)
      v.erase(iter++);
    else
      iter++;
  }

  for (int i = 0; i < v.size(); ++i)
    cout << v[i] << endl;
}

int main()
{
  test_erase();
}
