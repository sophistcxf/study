/*************************************************************************
  > File Name: test_log.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Mon 13 Jun 2016 02:35:42 PM CST
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
  struct Filter
  {
    bool operator()(const attribute_value_set& set)
    {
      return set["Name"].extract<std::string>() == name && set["Severity"].extract<int>() >= level; 
    }
    std::string name;
    unsigned int level;
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
  std::string format_;
};

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
  log::core::get()->add_sink(sink_);
}

bool only_warnings(const attribute_value_set& set)
{
  return set["Severity"].extract<int>() > 0 && set["Name"].extract<std::string>() == "Lisi";
}

class OnlyMe
{
public:
  bool operator ()(const attribute_value_set& set)
  {
    return set["Name"].extract<std::string>() == name;
  }
  std::string name;
};

bool only_me(const attribute_value_set& set)
{
  return set["Name"].extract<std::string>() == "Zhangsan";
}

void severity_and_message(const record_view &view, formatting_ostream &os)
{
  os << view.attribute_values()["Severity"].extract<int>() << " : " <<
        view.attribute_values()["Message"].extract<std::string>() << " : " << 
        view.attribute_values()["LineCounter"].extract<int>() << " : " <<
        view.attribute_values()["Constant"].extract<std::string>() << " : " <<
        view.attribute_values()["Mutable constant"].extract<int>() << " : " <<
        view.attribute_values()["Timer"].extract<int>() << " : " <<
        view.attribute_values()["Timestamp"].extract<std::string>();
}

void init()
{
  typedef log::sinks::synchronous_sink< log::sinks::text_ostream_backend > text_sink;
  boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

  sink->locked_backend()->add_stream(
      boost::make_shared< std::ofstream >("sample.log"));

  sink->set_formatter
    (
     expr::stream
     // line id will be written in hex, 8-digits, zero-filled
     << std::hex << std::setw(8) << std::setfill('0') << expr::attr< unsigned int >("LineID")
     << ": <" << logging::trivial::severity
     << "> " << expr::smessage << " : " << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
    );

  logging::core::get()->add_sink(sink);
}

void fun2()
{
  init();
  logging::add_common_attributes();
  using namespace logging::trivial;
  src::severity_logger< severity_level > lg;
  BOOST_LOG_SEV(lg, trace) << "A trace severity message";
  BOOST_LOG_SEV(lg, debug) << "A debug severity message";
  BOOST_LOG_SEV(lg, info) << "An informational severity message";
  BOOST_LOG_SEV(lg, warning) << "A warning severity message";
  BOOST_LOG_SEV(lg, error) << "An error severity message";
  BOOST_LOG_SEV(lg, fatal) << "A fatal severity message";
}

void fun1()
{
  typedef log::sinks::asynchronous_sink<log::sinks::text_ostream_backend> text_sink;
  boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();
  boost::shared_ptr<std::ostream> stream(&std::clog, boost::null_deleter());
  sink->locked_backend()->add_stream(stream);
  sink->locked_backend()->add_stream(boost::shared_ptr<std::ostream>(boost::make_shared<ofstream>("sink.log", ios_base::out)));
  sink->set_filter(&only_warnings);
  sink->set_formatter(&severity_and_message);
  typedef log::sinks::synchronous_sink<log::sinks::text_ostream_backend> s_text_sink;
  boost::shared_ptr<s_text_sink> s_sink = boost::make_shared<s_text_sink>();
  s_sink->locked_backend()->add_stream(boost::shared_ptr<std::ostream>(boost::make_shared<ofstream>("s_sink.log", ios_base::out)));
  OnlyMe zhangsan;
  zhangsan.name = "Zhangsan";
  s_sink->set_filter(zhangsan);
  log::core::get()->add_sink(s_sink);
  log::core::get()->add_sink(sink);
  log::core::get()->add_global_attribute("LineCounter", attributes::counter<int>());
  log::core::get()->add_global_attribute("Timestamp", attributes::local_clock());
  sources::severity_logger<int> lg;
  lg.add_attribute("Constant", attributes::constant<std::string>("Constant"));
  attributes::mutable_constant<int> m_const(-5);
  lg.add_attribute("Mutable constant", m_const);
  lg.add_attribute("Timer", attributes::timer());
  lg.add_attribute("Name", attributes::constant<std::string>("Zhangsan"));
  BOOST_LOG(lg) << "note";
  BOOST_LOG_SEV(lg, 0) << "sev 0";
  BOOST_LOG_SEV(lg, 1) << "sev 1";
  m_const.set(5);
  BOOST_LOG_SEV(lg, 2) << "sev 2";
  sink->flush();
}

void fun3()
{
  boost::shared_ptr< logging::core > core = logging::core::get();
  boost::shared_ptr< log::sinks::text_file_backend > backend =
    boost::make_shared< log::sinks::text_file_backend >(
        log::keywords::file_name = "file_%5N.log",                                          
        log::keywords::rotation_size = 5 * 1024 * 1024,                                    
        log::keywords::time_based_rotation = log::sinks::file::rotation_at_time_point(12, 0, 0)
        );
  typedef log::sinks::synchronous_sink< log::sinks::text_file_backend > sink_t;
  boost::shared_ptr< sink_t > sink(new sink_t(backend));
  core->add_sink(sink);
  sink->set_formatter(&severity_and_message);           
  log::core::get()->add_sink(sink);                     
  log::core::get()->add_global_attribute("LineCounter", attributes::counter<int>());
  log::core::get()->add_global_attribute("Timestamp", attributes::local_clock());
  sources::severity_logger<int> lg;                     
  lg.add_attribute("Constant", attributes::constant<std::string>("Constant"));
  attributes::mutable_constant<int> m_const(-5);        
  lg.add_attribute("Mutable constant", m_const);        
  lg.add_attribute("Timer", attributes::timer());       
  BOOST_LOG(lg) << "note";                              
  BOOST_LOG_SEV(lg, 0) << "sev 0";                      
  BOOST_LOG_SEV(lg, 1) << "sev 1";                      
  m_const.set(5);      
  BOOST_LOG_SEV(lg, 2) << "sev 2";                      
  sink->flush();       
}

int main()
{
  fun3();
  return 0;
}
