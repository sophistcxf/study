/*************************************************************************
  > File Name: /home/xiangfei.cui/workspace/mapmatching/test/generator/src/buffer_filter.c
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Thu 19 May 2016 03:36:44 PM CST
 ************************************************************************/

#include <stdio.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdlib.h>
using namespace std;

#include "buffer_filter.h"

#include "geos/geom/Coordinate.h"
#include "geos/operation/buffer/BufferOp.h"
#include "geos/geom/Geometry.h"
#include "geos/geom/LineString.h"
#include "geos/geom/CoordinateSequence.h"
#include "geos/geom/CoordinateArraySequence.h"
#include "geos/geom/GeometryFactory.h"
#include "geos/geom/Point.h"

bool stringToCoordinateList(char* line_vertex, vector<geos::geom::Coordinate>* coor_list);

/*!
 * 将line_vertex表示成LineString，计算gps_list有哪些点落在此LineString和distance形成的buffer中
 * 返回落在和不落在的索引列表
 * @param handler 存入返回的字符串
 * @param line_vertex 形成LineString的点，格式为x1,y1;x2,y2;...;xn,yn
 * @param distance 在LineString上创建buffer所用的distance
 * @param end_cap_style 在LineString上创建的buffer的类型. 1-CAP_ROUND, 2-CAP_FLAT, 3-CAP_SQUARE
 * @param gps_list GPS列表
 * @return 返回落在和不落在的索引，格式为i1,i2,i3,...,im;im+1,im+2,...,in，分号前是落在的索引，分号后是不落在的索引
 */
char* isContain(DataHandler* handler, char* line_vertex, double distance, unsigned end_cap_style, char* gps_list)
{
  if (handler == NULL || line_vertex == NULL || gps_list == NULL || distance == 0 || 
      (end_cap_style != 1 && end_cap_style != 2 && end_cap_style != 3)) 
      return NULL;
  vector<int> contain, not_contain;
  isContainEx(line_vertex, distance, end_cap_style, gps_list, contain, not_contain);
  ostringstream oss; 
  for (size_t i = 0; i < contain.size(); ++i) { 
    oss << contain[i]; 
    if (i != contain.size() - 1) oss << ",";
  }
  oss << ";";
  for (size_t i = 0; i < not_contain.size(); ++i) {
    oss << not_contain[i];
    if (i != not_contain.size() - 1) oss << ",";
  }
  handler->str_data = oss.str();
  return (char*)handler->str_data.c_str();
}

void isContainEx(char* line_vertex, double distance, int end_cap_style, char* gps_list, vector<int>& contain, vector<int>& not_contain)
{
  vector<geos::geom::Coordinate>* line_vertex_coor_list = new vector<geos::geom::Coordinate>;
  stringToCoordinateList(line_vertex, line_vertex_coor_list);
  vector<geos::geom::Coordinate>* gps_list_coor_list = new vector<geos::geom::Coordinate>;
  stringToCoordinateList(gps_list, gps_list_coor_list);
  geos::geom::CoordinateArraySequence cas(line_vertex_coor_list, 2);
  const geos::geom::GeometryFactory* gfc = geos::geom::GeometryFactory::getDefaultInstance();
  geos::geom::LineString* line_string = gfc->createLineString(cas);
  const int segments = 10;
  geos::geom::Geometry* buffer = geos::operation::buffer::BufferOp::bufferOp((geos::geom::Geometry*)line_string, distance, segments, end_cap_style);
  for (size_t i = 0; i < gps_list_coor_list->size(); ++i) {
    geos::geom::Point* pnt = gfc->createPoint(gps_list_coor_list->at(i));
    if (buffer->contains((geos::geom::Geometry*)pnt))
      contain.push_back(i);
    else
      not_contain.push_back(i);
  }
}

/*!
 * 将line_vertex，将字符串的点列表，转化为vector
 * @param line_vertex 格式为x1,y1;x2,y2
 */
bool stringToCoordinateList(char* line_vertex, vector<geos::geom::Coordinate>* coor_list)
{
  char* p = line_vertex;
  char* data = p;
  const unsigned len = strlen(line_vertex);
  char* pose = line_vertex + len;
  while (p < pose) {
    data = p; while (*p != ',' && ++p < pose) {} *p++ = 0;
    float x = atof(data);
    data = p; while (*p != ';' && ++p < pose) {} *p++ = 0;
    float y = atof(data);
    coor_list->push_back(geos::geom::Coordinate(x, y));
  }
  return true;
}

DataHandler* createDataHandler()
{
  return new DataHandler;
}

void destroyDataHandler(DataHandler* handler)
{
  if (handler != NULL) delete handler;
}
