/*************************************************************************
  > File Name: mylib.h
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Thu 19 May 2016 02:12:35 PM CST
 ************************************************************************/

#include <iostream>
#include <string>
using namespace std;

extern "C"
{
  struct DataHandler
  {
    std::string str;
  };
  struct Result
  {
    // 这种方法name的长度就定死了
    char name[20];
    int age;
  };
  int add(int a, int b);
  Result* getResult(char* name, int age);
  DataHandler* createDataHandler();
  void destroyDataHandler(DataHandler* handler);
  const char* outputString(DataHandler* handler, int line_num, int* out_len);
}
