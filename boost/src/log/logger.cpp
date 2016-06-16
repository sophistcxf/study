/*************************************************************************
  > File Name: logger.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 15 Jun 2016 10:39:30 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include "logger.h"

std::string LoggerEx::LEVEL_HANDLER[6] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

void LoggerEx::initialize(const std::string& name, unsigned int level, const std::string& path, long long unsigned size, bool daily_rolling, const std::string& format)
{
  boost::shared_ptr<log::sinks::text_file_backend> backend;
  ostringstream oss;
  oss << path << "_%Y%m%d_%H%M%S.log";
  backend = boost::make_shared<log::sinks::text_file_backend>(
      log::keywords::auto_flush = true,
      log::keywords::file_name = oss.str(),
      log::keywords::open_mode = (std::ios::out | std::ios::app)
      );
  if (daily_rolling) 
    backend->set_time_based_rotation(log::sinks::file::rotation_at_time_point(23, 59, 59));
  if (size > 0)
    backend->set_rotation_size(size * 1024 * 1024);

  sink_ = boost::make_shared<LoggerEx::text_sink>(backend); 
  filter_.name = name;
  filter_.level = level;
  formatter_.format = format;
  sink_->set_filter(filter_);
  logger_.add_attribute("Name", attributes::constant<std::string>(name));
  sink_->set_formatter(formatter_);
  log::core::get()->add_sink(sink_);
}
