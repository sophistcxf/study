/*************************************************************************
  > File Name: test_parse_xml.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 2016年12月07日 星期三 16时04分53秒
 ************************************************************************/

#include <string>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>

void test1();

int main()
{
  test1();
  return 0;
}

void test1()
{
  std::string config_path = "test.xml";
  boost::property_tree::ptree ptree;
  boost::property_tree::xml_parser::read_xml(config_path.c_str(), ptree);
  for (boost::property_tree::ptree::iterator it = ptree.begin(); it != ptree.end(); ++it)
  {
    std::cout << it->first << ":" << it->second.get_value<std::string>() << std::endl;
  }
}
