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


class Logger
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
    Formatter() : format("%d|%s|%p|%t|%s") {}
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
  Logger(std::string name, unsigned int level = 0, std::string path = "./log", unsigned int size = 50, bool daily_rolling = true, std::string format = "%d|%s|%p|%t|%s"); 
  void trace(const char* str_format, ...) { BOOST_LOG_SEV(logger_, 0) << str_format; }
  void info(const char* str_format, ...) { }
  void debug(const char* str_format, ...);
  void warn(const char* str_format, ...);
  void error(const char* str_format, ...);
  void fatal(const char* str_format, ...);
private:
  typedef log::sinks::synchronous_sink< log::sinks::text_file_backend > text_sink;
  boost::shared_ptr<text_sink> sink_;
  sources::severity_logger<int> logger_; 
  Filter filter_;
  Formatter formatter_;
};

std::string Logger::LEVEL_HANDLER[6] = {"TRACE", "INFO", "WARN", "ERROR", "FATAL"};

Logger::Logger(std::string name, unsigned int level, std::string path, unsigned int size, bool daily_rolling, std::string format)
{
  boost::shared_ptr<log::sinks::text_file_backend> backend;
  ostringstream oss;
  if (daily_rolling) {
    oss << path << "_%Y%m%d_%N.log";
    backend = boost::make_shared<log::sinks::text_file_backend>(
        log::keywords::file_name = oss.str(),
        log::keywords::rotation_size = size,
        log::keywords::time_based_rotation = log::sinks::file::rotation_at_time_point(11, 40, 0)
        );
  } else {
    oss << path << "_%N.log";
    backend = boost::make_shared<log::sinks::text_file_backend>(
        log::keywords::file_name = oss.str(),
        log::keywords::rotation_size = size
        );
  }
  sink_ = boost::make_shared<Logger::text_sink>(backend); 
  filter_.name = name;
  filter_.level = level;
  formatter_.format = format;
  sink_->set_filter(filter_);
  logger_.add_attribute("Name", attributes::constant<std::string>(name));
  sink_->set_formatter(formatter_);
  log::core::get()->add_sink(sink_);
}
