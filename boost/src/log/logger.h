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
#include <iostream>
#include <fstream>

using namespace boost;
using namespace boost::log;
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
    Formatter() : delimiter("|") {}
    void operator()(logging::record_view const& rec, logging::formatting_ostream& strm)
    {
    }
    std::string delimiter;
    
  };
public:
  Logger(std::string name, unsigned int level = 0, std::string path = "./log", unsigned int size = 50, bool daily_rolling = true, std::string format = ""); 
  void trace(const char* str_format, ...);
  void info(const char* str_format, ...);
  void debug(const char* str_format, ...);
  void warn(const char* str_format, ...);
  void error(const char* str_format, ...);
  void fatal(const char* str_format, ...);
private:
  typedef log::sinks::synchronous_sink< log::sinks::text_ostream_backend > text_sink;
  boost::shared_ptr<text_sink> sink_;
  sources::severity_logger<int> logger_; 
  Filter filter_;
  Formatter formatter_;
};

std::string Logger::LEVEL_HANDLER[6] = {"TRACE", "INFO", "WARN", "ERROR", "FATAL"};

Logger::Logger(std::string name, unsigned int level, std::string path, unsigned int size, bool daily_rolling, std::string format) :
  format_(format) 
{
  sink_ = boost::make_shared<Logger::text_sink>(); 
  filter_.name = name;
  filter_.level = level;
  sink_->set_filter(filter_);
  logger_.add_attribute("Name", attributes::constant<std::string>(name));
  boost::shared_ptr<log::sinks::text_file_backend> backend;
  ostringstream oss;
  if (daily_rolling) {
    oss << path << "_%Y%m%d_%H%M%S_%N.log";
    backend = boost::make_shared<log::sinks::text_file_backend>(
        log::keywords::file_name = oss.str(),
        log::keywords::rotation_size = size,
        log::keywords::time_based_rotation = log::sinks::file::rotation_at_time_point(12, 0, 0)
        );
  } else {
    oss << path << "_%N.log";
    backend = boost::make_shared<log::sinks::text_file_backend>(
        log::keywords::file_name = oss.str(),
        log::keywords::rotation_size = size
        );
  }
  sink_->set_formatter(formatter_);
  log::core::get()->add_sink(sink_);
}
