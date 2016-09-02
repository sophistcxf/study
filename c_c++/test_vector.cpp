/*************************************************************************
  > File Name: test_vector.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Fri 05 Aug 2016 05:57:06 PM CST
 ************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
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

bool compare(int n1, int n2)
{
  return n1 < n2;
}

void test_compare()
{
  vector<int> vec;
  vec.push_back(10);
  vec.push_back(3);
  vec.push_back(5);
  vec.push_back(6);
  vec.push_back(10);
  std::stable_sort(vec.begin(), vec.end(), compare);
  for (int i = 0; i < vec.size(); ++i)
    cout << vec[i] << endl;
}

int main()
{
  test_compare();
}
