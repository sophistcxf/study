#!/usr/bin/python
import sys
from datetime import datetime
import traceback
import logging
import os
import MySQLdb
from optparse import OptionParser
from thrift import Thrift
from thrift.protocol import TBinaryProtocol
from thrift.transport import TSocket, TTransport
from tinfo.mm_server.MatchServer import Client
from tinfo.mm_server.ttypes import Point
from ctypes import *

file_read_path = ""
BUFFER_FILTER_LIB_PATH = ""
db = MySQLdb.connect(host="127.0.0.1", user="mapmatching", passwd="tinfo_mm", db="match_road_system")

MM_SERVER_HOST = "127.0.0.1"
MM_SERVER_PORT = 9191

transport = TSocket.TSocket(MM_SERVER_HOST, MM_SERVER_PORT)
transport = TTransport.TBufferedTransport(transport)
protocol = TBinaryProtocol.TBinaryProtocol(transport)
client = Client(protocol)
transport.open()

buffer_filter = cdll.LoadLibrary("../libbuffer_filter.so")

case_id = int(sys.argv[1])

cursor = db.cursor()
sql = "select uncompress(gpslist), uncompress(true_link_list) from tbl_match_road_samples_detail where case_id=%d" % case_id
cursor.execute(sql)
content = cursor.fetchall()

gpslist = content[0][0]
true_link_list = content[0][1]

gps_vertex = []
for gps in gpslist.split(";"):
  gps_fields = gps.split(",")
  gps_vertex.append("%s,%s" % (gps_fields[1], gps_fields[2]))
gps_vertex = ";".join(gps_vertex)

line_vertex = []
for link in map(int, true_link_list.split(",")):
  link_info = client.linkInfoOfID(link)
  for coor in link_info.coors:
    line_vertex.append("%s,%s" % (str(coor.x), str(coor.y)))
line_vertex = ";".join(line_vertex)
distance = c_double(0.001)
handler = buffer_filter.createDataHandler()
data = buffer_filter.isContain(handler, line_vertex, distance, 1, gps_vertex)
rst = string_at(data)
contain_list = rst.split(";")[0].strip();
not_contain_list = rst.split(";")[1]
contain_cnt = 0 
if contain_list != "":
  contain_cnt = len(rst.split(";")[0].split(","))
not_contain_cnt = 0
if not_contain_list != "":
  not_contain_cnt = len(rst.split(";")[1].split(","))
print contain_cnt * 1.0 / (contain_cnt + not_contain_cnt)
