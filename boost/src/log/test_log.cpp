/*************************************************************************
  > File Name: test_log.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Mon 13 Jun 2016 02:35:42 PM CST
 ************************************************************************/

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
        log::keywords::time_based_rotation = log::sinks::file::rotation_at_time_point(12, 0, 0),
        log::keywords::format = "[%TimeStamp%]: %Message%"
        );
  typedef log::sinks::synchronous_sink< log::sinks::text_file_backend > sink_t;
  boost::shared_ptr< sink_t > sink(new sink_t(backend));
  core->add_sink(sink);
  //sink->set_formatter(&severity_and_message);           
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
