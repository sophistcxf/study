/*************************************************************************
  > File Name: logger.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 15 Jun 2016 10:39:30 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include "logger.h"

int main()
{
  Logger logger("Cuixiangfei", 0, "./cxf.log", 50, true);
  while (true) {
    logger.trace("Trace");
    sleep(1);
  }
  return 0;
}
