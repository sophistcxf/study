/*************************************************************************
  > File Name: test_to_posix_string.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 15 Jun 2016 10:12:11 PM CST
 ************************************************************************/

#include <iostream>
#include <string>
using namespace std;
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

int main()
{
  std::string strTime = boost::gregorian::to_iso_string(boost::gregorian::day_clock::local_day());  
  std::cout << strTime << std::endl;


  strTime = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
  strTime.replace(strTime.find('T'), 1, "_");
  std::cout << strTime << std::endl;

  strTime = boost::posix_time::to_iso_string(boost::posix_time::from_time_t(1479802843));
  std::cout << strTime << std::endl;

  strTime = boost::posix_time::to_iso_string( boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(boost::posix_time::from_time_t(1479802843)));

  return 0;
}
