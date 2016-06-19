/*************************************************************************
  > File Name: buffer_filter.h
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Thu 19 May 2016 02:50:42 PM CST
 ************************************************************************/

#include <iostream>
#include <vector>
using namespace std;

extern "C"
{
  struct DataHandler
  {
    std::string str_data;
  };
  char* isContain(DataHandler* handler, char* line_vertex, double distance, unsigned end_cap_style, char* gps_list);
  void isContainEx(char* line_vertex, double distance, int end_cap_style, char* gps_list, vector<int>& contain, vector<int>& not_contain);
  DataHandler* createDataHandler();
  void destroyDataHandler(DataHandler* handler);
}
