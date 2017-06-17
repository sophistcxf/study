/*************************************************************************
  > File Name: test_numeric_limits.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 2016年09月17日 星期六 12时38分16秒
 ************************************************************************/

#include <iostream>
#include <limits>
using namespace std;

int main()
{
  cout << std::numeric_limits<int>::max() << "\t" << std::numeric_limits<int>::min() << endl;
  cout << std::numeric_limits<unsigned>::max() << "\t" << std::numeric_limits<unsigned>::min() << endl;
  cout << std::numeric_limits<float>::max() << "\t" << std::numeric_limits<float>::min() << endl;
  cout << std::numeric_limits<double>::max() << "\t" << std::numeric_limits<double>::min() << endl;
  if (std::numeric_limits<float>::min() < 0)
  cout << "dd";
}
