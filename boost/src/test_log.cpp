/*************************************************************************
  > File Name: test_log.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Mon 13 Jun 2016 02:35:42 PM CST
 ************************************************************************/

#include <iostream>
#include <string>
using namespace std;

#include <boost/log/common.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

using namespace boost::log;

class Logger
{
public:
  Logger(unsigned int level = 0, std::string path = "./log", unsigned int size = 50, bool daily_rolling = true, std::string format = "") :
    level_(level), path_(path), max_size_(size), daily_rolling_(daily_rolling), format_(format) {}
  void setLevel(unsigned int level) { level_ = level; }
  void setPath(std::string path) { path_ = path; } 
  void setMaxSize(unsigned int size) { max_size_ = size; }
  void setDailyRolling(bool daily_rolling) { daily_rolling_ = daily_rolling; }
  void setFormat(std::string format) { format_ = format; }
  void trace(const char* str_format, ...);
  void info(const char* str_format, ...);
  void debug(const char* str_format, ...);
  void warn(const char* str_format, ...);
  void error(const char* str_format, ...);
  void fatal(const char* str_format, ...);
private:
  unsigned int level_;
  std::string path_;
  unsigned int max_size_;
  bool daily_rolling_;
  std::string format_;
};

bool only_warnings(const attribute_value_set& set)
{
  return set["Severity"].extract<int>() > 0;
}

int main()
{
  typedef sinks::asynchronous_sink<sinks::text_ostream_backend> text_sink;
  boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();
  boost::shared_ptr<std::ostream> stream(&std::clog, boost::null_deleter());
  sink->locked_backend()->add_stream(stream);
  core::get()->add_sink(sink);
  sources::logger lg;
  BOOST_LOG(lg) << "note";
  sink->flush();
}
