/*************************************************************************
  > File Name: test_archive.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 2016年10月15日 星期六 15时22分25秒
 ************************************************************************/

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>
#include <fstream>

using namespace boost::archive;

void save()
{
  std::ofstream file("archive.txt", std::ofstream::out);
  text_oarchive oa(file);
  int i = 1;
  oa << i;
}

void load()
{
  std::ifstream file("archive.txt", std::ifstream::in);
  text_iarchive ia(file);
  int i = 0;
  ia >> i;
  std::cout << i << '\n';
}

int main()
{
  save();
  load();
}
