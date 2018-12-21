/*************************************************************************
  > File Name: logger.h
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 15 Jun 2016 06:37:21 PM CST
 ************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <time.h>
#include <stdarg.h>
using namespace std;

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/common.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/attributes/constant.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <fstream>

using namespace boost;
using namespace boost::log;
using namespace boost::algorithm;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;


class LoggerEx
{
  static std::string LEVEL_HANDLER[6];
  struct Filter
  {
    Filter() : level(0) {}
    bool operator()(const attribute_value_set& set)
    {
      return set["Severity"].extract<int>() >= level && 
             (set["Name"].extract<std::string>() == name || name.empty()) ; 
    }
    std::string name;
    unsigned int level;
  };
  struct Formatter
  {
    Formatter() : format("%d|%s|%p|%t|%m") {}
    void operator()(logging::record_view const& rec, logging::formatting_ostream& strm)
    {
      std::string temp = format;
      struct tm t;
      time_t time = ::time(NULL);
      localtime_r(&time, &t);
      char timestamp[24] = {0};
      strftime(timestamp, 24, "%Y-%m-%d %H:%M:%S", &t); 
      replace_all(temp, "%d", std::string(timestamp));
      replace_all(temp, "%s", LEVEL_HANDLER[*rec.attribute_values()["Severity"].extract<int>()]);
      replace_all(temp, "%m", *(rec.attribute_values()["Message"].extract<std::string>()));
      ostringstream oss;
      oss << boost::this_thread::get_id();
      replace_all(temp, "%t", oss.str().c_str()); 
      oss << getpid();
      replace_all(temp, "%p", oss.str().c_str()); 
      strm << temp;
    }
    /*
     * %d 时间戳 YYYY-mm-dd HH:MM:SS
     * %s Severity
     * %p pid
     * %t tid
     * %m message
     */
    std::string format; 
  };
public:
  LoggerEx(const std::string& name, unsigned int level = 0, const std::string& path = "./log", unsigned long long size = 0, bool daily_rolling = true, const std::string& format = "%d|%s|%p|%t|%m")
  {
    initialize(name, level, path, size, daily_rolling, format);
  }
  void initialize(const std::string& name, unsigned int level = 0, const std::string& path = "./log", unsigned long long size = 0, bool daily_rolling = true, const std::string& format = "%d|%s|%p|%t|%m"); 

#define MSG_FORMAT(level)                                             \
  do {                                                                \
    va_list args;                                                     \
    va_start(args, str_format);                                       \
    BOOST_LOG_SEV(logger_, level) << format_msg(str_format, args);    \
    va_end(args);                                                     \
  } while(false);

  void trace(const char* str_format, ...) 
  { 
    MSG_FORMAT(0)
  }
  void debug(const char* str_format, ...) 
  {
    MSG_FORMAT(1)
  }
  void info(const char* str_format, ...) 
  {
    MSG_FORMAT(2)
  }
  void warn(const char* str_format, ...)
  {
    MSG_FORMAT(3)
  }
  void error(const char* str_format, ...)
  { 
    MSG_FORMAT(4)
  }
  void fatal(const char* str_format, ...)
  {
    MSG_FORMAT(5)
  }
private:
  std::string format_msg(const char* fmt, va_list& args)
  {
    va_list args2;
    va_copy(args2, args);
    vector<char> buf(vsnprintf(NULL, 0, fmt, args) + 1);
    int len = vsnprintf(&buf[0], buf.size(), fmt, args2);
    va_end(args2);
    return std::string(&buf[0], len);
  }
  typedef log::sinks::synchronous_sink< log::sinks::text_file_backend > text_sink;
  boost::shared_ptr<text_sink> sink_;
  sources::severity_logger<int> logger_; 
  Filter filter_;
  Formatter formatter_;
};
