/*************************************************************************
  > File Name: tutorial_fmt_stream.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Wed 15 Jun 2016 01:24:35 PM CST
 ************************************************************************/

/*
 *          Copyright Andrey Semashev 2007 - 2015.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

void init()
{
  logging::add_file_log
    (
     keywords::file_name = "sample_%N.log",
     // This makes the sink to write log records that look like this:
     // YYYY-MM-DD HH:MI:SS: <normal> A normal severity message
     // YYYY-MM-DD HH:MI:SS: <error> An error severity message
     keywords::format =
     (
      expr::stream
      << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
      << ": <" << logging::trivial::severity
      << "> " << expr::smessage
     )
    );
}

int main(int, char*[])
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

  return 0;
}
