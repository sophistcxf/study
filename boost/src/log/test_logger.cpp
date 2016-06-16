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
  //Logger logger("Cuixiangfei", 0, "./cxf.log", 1024 * 1024 * 1024, true);
  LoggerEx logger("Cuixiangfei", 0, "./cxf.log", 1024 * 1024 * 1024, true, "%d | %t | %s | %m");
  int i = 0;
  while (true) {
    logger.trace("Trace %d", 10);
    logger.fatal("fatal %s, %d", "abc", 10);
    sleep(1);
 //   if (i++ == 10) break
  }
  return 0;
}
