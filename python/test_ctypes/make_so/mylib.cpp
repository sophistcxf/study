/*************************************************************************
  > File Name: mylib.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Thu 19 May 2016 02:13:23 PM CST
 ************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
using namespace std;

#include "mylib.h"

int add(int a, int b)
{
  return a + b;
}

Result* getResult(char* name, int age)
{
  Result* rlt = (Result*)malloc(sizeof(Result));
  strcpy(rlt->name, name);
  rlt->age = age;
  return rlt;
}

DataHandler* createDataHandler()
{
  return new DataHandler;
}

void destroyDataHandler(DataHandler* handler)
{
  if (handler != NULL) delete handler;
}

/* 使用DataHandler的原因是出了outputString的作用域，str就没有了，所以要有个地方持续保留
 * str的内存，直到调用了destroyDataHandler
 */
const char* outputString(DataHandler* handler, int line_num, int* out_len)
{
  std::stringstream ss;
  for (int i = 0; i < line_num; ++i) {
    ss << "This is line " << i << "\n";
  }
  handler->str = ss.str();
  *out_len = handler->str.size();
  return handler->str.c_str();
}
