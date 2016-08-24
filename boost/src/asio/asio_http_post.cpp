/*************************************************************************
  > File Name: asio_http_post.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 2016年08月24日 星期三 20时10分15秒
 ************************************************************************/

/*
 * 使用boost asio实现的http post
 */

#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_service.hpp>

int post(const std::string& host, const std::string& port, const std::string& page, const std::string& post_data, std::string& res_data)
{
  try {
    boost::asio::io_service io_service;
    if (io_service.stopped()) io_service.reset();
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(host, port);
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    boost::asio::ip::tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);
    // 拼http post请求的内容
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "POST " << page << " HTTP/1.0\r\n";
    request_stream << "Host: " << host << ":" << port << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Content-Length: " << post_data.length() << "\r\n";
    request_stream << "Content-Type: application/x-www-form-urlencoded\r\n";
    request_stream << "Connection: close\r\n";
    request_stream << "\r\n";
    request_stream << post_data;
    // send the request
    boost::asio::write(socket, request);
    // wait for response
    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n");
    // get http version and return code 
    std::istream response_stream(&response);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
      res_data = "Invalid response";
      return -2;
    }
    if (status_code != 200) {
      res_data = "Response returned with status code != 200 " ;
      return status_code;
    }
    std::string header;
    std::vector<std::string> headers;        
    while (std::getline(response_stream, header) && header != "\r")
      headers.push_back(header);
    boost::system::error_code error;
    while (boost::asio::read(socket, response,
          boost::asio::transfer_at_least(1), error))
    {           
    }
    if (response.size() > 0) {
      std::istream response_stream(&response);
      std::istreambuf_iterator<char> eos;
      res_data = std::string(std::istreambuf_iterator<char>(response_stream), eos);                        
    }
    if (error != boost::asio::error::eof) {
      res_data = error.message();
      return -3;
    }
  } catch (std::exception& e) {
    res_data = e.what();
    return -1;
  }
  return 0;
}

int main(int argc, char* argv[])
{
  std::string host = "dict.youdao.com";   
  std::string port = "80";
  std::string page = "/w/hello/";
  std::string data = "#keyfrom=dict2.top";
  std::string reponse_data;
  int ret = post(host, port, page, data, reponse_data);
  if (ret != 0)
    std::cout << "error_code:" << ret << std::endl;
  std::cout << reponse_data << std::endl;
  return 0;
}
