/*************************************************************************
  > File Name: test_gzip.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 2016年08月29日 星期一 16时39分50秒
 ************************************************************************/

#include <iostream>
using namespace std;

#include <stdlib.h>  
  
#include <iostream>  
#include <sstream>  
#include <fstream>
#include <vector>  

#include <boost/iostreams/filtering_streambuf.hpp>    
#include <boost/iostreams/filtering_stream.hpp>    
#include <boost/iostreams/copy.hpp>    
#include <boost/iostreams/filter/gzip.hpp>    
#include <boost/iostreams/filter/zlib.hpp>

void test_gzip();
void test_gzip2();

int main()
{
  test_gzip2();
  return 0;
}

void test_gzip()
{
  istringstream iss("hello");  
  stringstream ss;  
  boost::iostreams::filtering_streambuf<boost::iostreams::input> in;  
  in.push(boost::iostreams::gzip_compressor());  
  in.push(iss);  
  boost::iostreams::copy(in, ss);
  ofstream ofs("test.gz");
  ofs << ss.str();
}

void test_gzip2()
{
  boost::iostreams::filtering_ostream out;
  std::string compressedString;
  out.push(boost::iostreams::gzip_compressor());
  out.push(boost::iostreams::back_inserter(compressedString));
  out << "hello";
  out << " world!";
  // 没有push方法，filtering_streambuf和filtering_ostream的区别
  //out.push(" cxxxxf\n");
  boost::iostreams::close(out);
  ofstream ofs("test.gz");
  ofs << compressedString;
}
