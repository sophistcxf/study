/*************************************************************************
  > File Name: adjacency_list_struct.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 2016年10月08日 星期六 15时23分35秒
 ************************************************************************/

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
using namespace std;

int main()
{
  boost::adjacency_list<> g;

  boost::adjacency_list<>::vertex_descriptor v1 = boost::add_vertex(g);
  boost::adjacency_list<>::vertex_descriptor v2 = boost::add_vertex(g);
  boost::adjacency_list<>::vertex_descriptor v3 = boost::add_vertex(g);
  boost::adjacency_list<>::vertex_descriptor v4 = boost::add_vertex(g);

  std::cout << v1 << ", " << v2 << ", " << v3 << ", " << v4 << '\n';
}
