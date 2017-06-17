/*************************************************************************
  > File Name: test.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Thu 19 May 2016 05:17:20 PM CST
 ************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "../buffer_filter.h"

int main(int argc, char* argv[])
{
  if (argc < 2) return -1;
  ifstream ifs(argv[1], ios_base::in);
  string line_vertex, gpslist;
  ifs >> line_vertex >> gpslist;
  vector<int> contain, not_contain;
  isContainEx((char*)line_vertex.c_str(), 0.001, 1, (char*)gpslist.c_str(), contain, not_contain);
  //DataHandler handler;
  //char* p = isContain(&handler, (char*)line_vertex.c_str(), 80, 1, (char*)gpslist.c_str());
  cout << "contains: ";
  for (size_t i = 0; i < contain.size(); ++i)  
    cout << contain[i] << "\t";
  cout << endl;
  cout << "not containts: ";
  for (size_t i = 0; i < not_contain.size(); ++i)
    cout << not_contain[i] << "\t";
  cout << endl;
  return 0;
}
