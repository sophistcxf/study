#
# Autogenerated by Thrift Compiler (0.9.3)
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#
#  options string: py
#

from thrift.Thrift import TType, TMessageType, TException, TApplicationException

from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol, TProtocol
try:
  from thrift.protocol import fastbinary
except:
  fastbinary = None


class PointMatchType:
  kPointMatchTypeSuccess = 0
  kPointMatchTypeStopPoint = 1
  kPointMatchTypeMatchFailed = 2
  kPointMatchTypeRouteFailed = 3

  _VALUES_TO_NAMES = {
    0: "kPointMatchTypeSuccess",
    1: "kPointMatchTypeStopPoint",
    2: "kPointMatchTypeMatchFailed",
    3: "kPointMatchTypeRouteFailed",
  }

  _NAMES_TO_VALUES = {
    "kPointMatchTypeSuccess": 0,
    "kPointMatchTypeStopPoint": 1,
    "kPointMatchTypeMatchFailed": 2,
    "kPointMatchTypeRouteFailed": 3,
  }


class Point:
  """
  Attributes:
   - tm
   - x
   - y
   - speed
   - dire
  """

  thrift_spec = (
    None, # 0
    (1, TType.I64, 'tm', None, None, ), # 1
    (2, TType.DOUBLE, 'x', None, None, ), # 2
    (3, TType.DOUBLE, 'y', None, None, ), # 3
    (4, TType.DOUBLE, 'speed', None, -1, ), # 4
    (5, TType.I16, 'dire', None, -1, ), # 5
  )

  def __init__(self, tm=None, x=None, y=None, speed=thrift_spec[4][4], dire=thrift_spec[5][4],):
    self.tm = tm
    self.x = x
    self.y = y
    self.speed = speed
    self.dire = dire

  def read(self, iprot):
    if iprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None and fastbinary is not None:
      fastbinary.decode_binary(self, iprot.trans, (self.__class__, self.thrift_spec))
      return
    iprot.readStructBegin()
    while True:
      (fname, ftype, fid) = iprot.readFieldBegin()
      if ftype == TType.STOP:
        break
      if fid == 1:
        if ftype == TType.I64:
          self.tm = iprot.readI64()
        else:
          iprot.skip(ftype)
      elif fid == 2:
        if ftype == TType.DOUBLE:
          self.x = iprot.readDouble()
        else:
          iprot.skip(ftype)
      elif fid == 3:
        if ftype == TType.DOUBLE:
          self.y = iprot.readDouble()
        else:
          iprot.skip(ftype)
      elif fid == 4:
        if ftype == TType.DOUBLE:
          self.speed = iprot.readDouble()
        else:
          iprot.skip(ftype)
      elif fid == 5:
        if ftype == TType.I16:
          self.dire = iprot.readI16()
        else:
          iprot.skip(ftype)
      else:
        iprot.skip(ftype)
      iprot.readFieldEnd()
    iprot.readStructEnd()

  def write(self, oprot):
    if oprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and self.thrift_spec is not None and fastbinary is not None:
      oprot.trans.write(fastbinary.encode_binary(self, (self.__class__, self.thrift_spec)))
      return
    oprot.writeStructBegin('Point')
    if self.tm is not None:
      oprot.writeFieldBegin('tm', TType.I64, 1)
      oprot.writeI64(self.tm)
      oprot.writeFieldEnd()
    if self.x is not None:
      oprot.writeFieldBegin('x', TType.DOUBLE, 2)
      oprot.writeDouble(self.x)
      oprot.writeFieldEnd()
    if self.y is not None:
      oprot.writeFieldBegin('y', TType.DOUBLE, 3)
      oprot.writeDouble(self.y)
      oprot.writeFieldEnd()
    if self.speed is not None:
      oprot.writeFieldBegin('speed', TType.DOUBLE, 4)
      oprot.writeDouble(self.speed)
      oprot.writeFieldEnd()
    if self.dire is not None:
      oprot.writeFieldBegin('dire', TType.I16, 5)
      oprot.writeI16(self.dire)
      oprot.writeFieldEnd()
    oprot.writeFieldStop()
    oprot.writeStructEnd()

  def validate(self):
    if self.tm is None:
      raise TProtocol.TProtocolException(message='Required field tm is unset!')
    if self.x is None:
      raise TProtocol.TProtocolException(message='Required field x is unset!')
    if self.y is None:
      raise TProtocol.TProtocolException(message='Required field y is unset!')
    if self.speed is None:
      raise TProtocol.TProtocolException(message='Required field speed is unset!')
    if self.dire is None:
      raise TProtocol.TProtocolException(message='Required field dire is unset!')
    return


  def __hash__(self):
    value = 17
    value = (value * 31) ^ hash(self.tm)
    value = (value * 31) ^ hash(self.x)
    value = (value * 31) ^ hash(self.y)
    value = (value * 31) ^ hash(self.speed)
    value = (value * 31) ^ hash(self.dire)
    return value

  def __repr__(self):
    L = ['%s=%r' % (key, value)
      for key, value in self.__dict__.iteritems()]
    return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

  def __eq__(self, other):
    return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

  def __ne__(self, other):
    return not (self == other)

class Coor:
  """
  Attributes:
   - x
   - y
  """

  thrift_spec = (
    None, # 0
    (1, TType.DOUBLE, 'x', None, None, ), # 1
    (2, TType.DOUBLE, 'y', None, None, ), # 2
  )

  def __init__(self, x=None, y=None,):
    self.x = x
    self.y = y

  def read(self, iprot):
    if iprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None and fastbinary is not None:
      fastbinary.decode_binary(self, iprot.trans, (self.__class__, self.thrift_spec))
      return
    iprot.readStructBegin()
    while True:
      (fname, ftype, fid) = iprot.readFieldBegin()
      if ftype == TType.STOP:
        break
      if fid == 1:
        if ftype == TType.DOUBLE:
          self.x = iprot.readDouble()
        else:
          iprot.skip(ftype)
      elif fid == 2:
        if ftype == TType.DOUBLE:
          self.y = iprot.readDouble()
        else:
          iprot.skip(ftype)
      else:
        iprot.skip(ftype)
      iprot.readFieldEnd()
    iprot.readStructEnd()

  def write(self, oprot):
    if oprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and self.thrift_spec is not None and fastbinary is not None:
      oprot.trans.write(fastbinary.encode_binary(self, (self.__class__, self.thrift_spec)))
      return
    oprot.writeStructBegin('Coor')
    if self.x is not None:
      oprot.writeFieldBegin('x', TType.DOUBLE, 1)
      oprot.writeDouble(self.x)
      oprot.writeFieldEnd()
    if self.y is not None:
      oprot.writeFieldBegin('y', TType.DOUBLE, 2)
      oprot.writeDouble(self.y)
      oprot.writeFieldEnd()
    oprot.writeFieldStop()
    oprot.writeStructEnd()

  def validate(self):
    if self.x is None:
      raise TProtocol.TProtocolException(message='Required field x is unset!')
    if self.y is None:
      raise TProtocol.TProtocolException(message='Required field y is unset!')
    return


  def __hash__(self):
    value = 17
    value = (value * 31) ^ hash(self.x)
    value = (value * 31) ^ hash(self.y)
    return value

  def __repr__(self):
    L = ['%s=%r' % (key, value)
      for key, value in self.__dict__.iteritems()]
    return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

  def __eq__(self, other):
    return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

  def __ne__(self, other):
    return not (self == other)

class PointMatchInfo:
  """
  Attributes:
   - point_match_type
   - project_pnt
   - candidate_links
   - start_off
   - end_off
  """

  thrift_spec = (
    None, # 0
    (1, TType.I32, 'point_match_type', None, None, ), # 1
    (2, TType.STRUCT, 'project_pnt', (Coor, Coor.thrift_spec), None, ), # 2
    (3, TType.LIST, 'candidate_links', (TType.I64,None), None, ), # 3
    (4, TType.I16, 'start_off', None, None, ), # 4
    (5, TType.I16, 'end_off', None, None, ), # 5
  )

  def __init__(self, point_match_type=None, project_pnt=None, candidate_links=None, start_off=None, end_off=None,):
    self.point_match_type = point_match_type
    self.project_pnt = project_pnt
    self.candidate_links = candidate_links
    self.start_off = start_off
    self.end_off = end_off

  def read(self, iprot):
    if iprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None and fastbinary is not None:
      fastbinary.decode_binary(self, iprot.trans, (self.__class__, self.thrift_spec))
      return
    iprot.readStructBegin()
    while True:
      (fname, ftype, fid) = iprot.readFieldBegin()
      if ftype == TType.STOP:
        break
      if fid == 1:
        if ftype == TType.I32:
          self.point_match_type = iprot.readI32()
        else:
          iprot.skip(ftype)
      elif fid == 2:
        if ftype == TType.STRUCT:
          self.project_pnt = Coor()
          self.project_pnt.read(iprot)
        else:
          iprot.skip(ftype)
      elif fid == 3:
        if ftype == TType.LIST:
          self.candidate_links = []
          (_etype3, _size0) = iprot.readListBegin()
          for _i4 in xrange(_size0):
            _elem5 = iprot.readI64()
            self.candidate_links.append(_elem5)
          iprot.readListEnd()
        else:
          iprot.skip(ftype)
      elif fid == 4:
        if ftype == TType.I16:
          self.start_off = iprot.readI16()
        else:
          iprot.skip(ftype)
      elif fid == 5:
        if ftype == TType.I16:
          self.end_off = iprot.readI16()
        else:
          iprot.skip(ftype)
      else:
        iprot.skip(ftype)
      iprot.readFieldEnd()
    iprot.readStructEnd()

  def write(self, oprot):
    if oprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and self.thrift_spec is not None and fastbinary is not None:
      oprot.trans.write(fastbinary.encode_binary(self, (self.__class__, self.thrift_spec)))
      return
    oprot.writeStructBegin('PointMatchInfo')
    if self.point_match_type is not None:
      oprot.writeFieldBegin('point_match_type', TType.I32, 1)
      oprot.writeI32(self.point_match_type)
      oprot.writeFieldEnd()
    if self.project_pnt is not None:
      oprot.writeFieldBegin('project_pnt', TType.STRUCT, 2)
      self.project_pnt.write(oprot)
      oprot.writeFieldEnd()
    if self.candidate_links is not None:
      oprot.writeFieldBegin('candidate_links', TType.LIST, 3)
      oprot.writeListBegin(TType.I64, len(self.candidate_links))
      for iter6 in self.candidate_links:
        oprot.writeI64(iter6)
      oprot.writeListEnd()
      oprot.writeFieldEnd()
    if self.start_off is not None:
      oprot.writeFieldBegin('start_off', TType.I16, 4)
      oprot.writeI16(self.start_off)
      oprot.writeFieldEnd()
    if self.end_off is not None:
      oprot.writeFieldBegin('end_off', TType.I16, 5)
      oprot.writeI16(self.end_off)
      oprot.writeFieldEnd()
    oprot.writeFieldStop()
    oprot.writeStructEnd()

  def validate(self):
    if self.point_match_type is None:
      raise TProtocol.TProtocolException(message='Required field point_match_type is unset!')
    if self.project_pnt is None:
      raise TProtocol.TProtocolException(message='Required field project_pnt is unset!')
    if self.candidate_links is None:
      raise TProtocol.TProtocolException(message='Required field candidate_links is unset!')
    if self.start_off is None:
      raise TProtocol.TProtocolException(message='Required field start_off is unset!')
    if self.end_off is None:
      raise TProtocol.TProtocolException(message='Required field end_off is unset!')
    return


  def __hash__(self):
    value = 17
    value = (value * 31) ^ hash(self.point_match_type)
    value = (value * 31) ^ hash(self.project_pnt)
    value = (value * 31) ^ hash(self.candidate_links)
    value = (value * 31) ^ hash(self.start_off)
    value = (value * 31) ^ hash(self.end_off)
    return value

  def __repr__(self):
    L = ['%s=%r' % (key, value)
      for key, value in self.__dict__.iteritems()]
    return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

  def __eq__(self, other):
    return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

  def __ne__(self, other):
    return not (self == other)

class MatchedTrace:
  """
  Attributes:
   - link_list
   - point_match_list
  """

  thrift_spec = (
    None, # 0
    (1, TType.LIST, 'link_list', (TType.I64,None), None, ), # 1
    (2, TType.LIST, 'point_match_list', (TType.STRUCT,(PointMatchInfo, PointMatchInfo.thrift_spec)), None, ), # 2
  )

  def __init__(self, link_list=None, point_match_list=None,):
    self.link_list = link_list
    self.point_match_list = point_match_list

  def read(self, iprot):
    if iprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None and fastbinary is not None:
      fastbinary.decode_binary(self, iprot.trans, (self.__class__, self.thrift_spec))
      return
    iprot.readStructBegin()
    while True:
      (fname, ftype, fid) = iprot.readFieldBegin()
      if ftype == TType.STOP:
        break
      if fid == 1:
        if ftype == TType.LIST:
          self.link_list = []
          (_etype10, _size7) = iprot.readListBegin()
          for _i11 in xrange(_size7):
            _elem12 = iprot.readI64()
            self.link_list.append(_elem12)
          iprot.readListEnd()
        else:
          iprot.skip(ftype)
      elif fid == 2:
        if ftype == TType.LIST:
          self.point_match_list = []
          (_etype16, _size13) = iprot.readListBegin()
          for _i17 in xrange(_size13):
            _elem18 = PointMatchInfo()
            _elem18.read(iprot)
            self.point_match_list.append(_elem18)
          iprot.readListEnd()
        else:
          iprot.skip(ftype)
      else:
        iprot.skip(ftype)
      iprot.readFieldEnd()
    iprot.readStructEnd()

  def write(self, oprot):
    if oprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and self.thrift_spec is not None and fastbinary is not None:
      oprot.trans.write(fastbinary.encode_binary(self, (self.__class__, self.thrift_spec)))
      return
    oprot.writeStructBegin('MatchedTrace')
    if self.link_list is not None:
      oprot.writeFieldBegin('link_list', TType.LIST, 1)
      oprot.writeListBegin(TType.I64, len(self.link_list))
      for iter19 in self.link_list:
        oprot.writeI64(iter19)
      oprot.writeListEnd()
      oprot.writeFieldEnd()
    if self.point_match_list is not None:
      oprot.writeFieldBegin('point_match_list', TType.LIST, 2)
      oprot.writeListBegin(TType.STRUCT, len(self.point_match_list))
      for iter20 in self.point_match_list:
        iter20.write(oprot)
      oprot.writeListEnd()
      oprot.writeFieldEnd()
    oprot.writeFieldStop()
    oprot.writeStructEnd()

  def validate(self):
    if self.link_list is None:
      raise TProtocol.TProtocolException(message='Required field link_list is unset!')
    if self.point_match_list is None:
      raise TProtocol.TProtocolException(message='Required field point_match_list is unset!')
    return


  def __hash__(self):
    value = 17
    value = (value * 31) ^ hash(self.link_list)
    value = (value * 31) ^ hash(self.point_match_list)
    return value

  def __repr__(self):
    L = ['%s=%r' % (key, value)
      for key, value in self.__dict__.iteritems()]
    return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

  def __eq__(self, other):
    return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

  def __ne__(self, other):
    return not (self == other)

class LinkInfo:
  """
  Attributes:
   - link_id
   - road_class
   - formway
   - length
   - kmph
   - coors
  """

  thrift_spec = (
    None, # 0
    (1, TType.I64, 'link_id', None, None, ), # 1
    (2, TType.I32, 'road_class', None, None, ), # 2
    (3, TType.I32, 'formway', None, None, ), # 3
    (4, TType.I32, 'length', None, None, ), # 4
    (5, TType.DOUBLE, 'kmph', None, None, ), # 5
    (6, TType.LIST, 'coors', (TType.STRUCT,(Coor, Coor.thrift_spec)), None, ), # 6
  )

  def __init__(self, link_id=None, road_class=None, formway=None, length=None, kmph=None, coors=None,):
    self.link_id = link_id
    self.road_class = road_class
    self.formway = formway
    self.length = length
    self.kmph = kmph
    self.coors = coors

  def read(self, iprot):
    if iprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None and fastbinary is not None:
      fastbinary.decode_binary(self, iprot.trans, (self.__class__, self.thrift_spec))
      return
    iprot.readStructBegin()
    while True:
      (fname, ftype, fid) = iprot.readFieldBegin()
      if ftype == TType.STOP:
        break
      if fid == 1:
        if ftype == TType.I64:
          self.link_id = iprot.readI64()
        else:
          iprot.skip(ftype)
      elif fid == 2:
        if ftype == TType.I32:
          self.road_class = iprot.readI32()
        else:
          iprot.skip(ftype)
      elif fid == 3:
        if ftype == TType.I32:
          self.formway = iprot.readI32()
        else:
          iprot.skip(ftype)
      elif fid == 4:
        if ftype == TType.I32:
          self.length = iprot.readI32()
        else:
          iprot.skip(ftype)
      elif fid == 5:
        if ftype == TType.DOUBLE:
          self.kmph = iprot.readDouble()
        else:
          iprot.skip(ftype)
      elif fid == 6:
        if ftype == TType.LIST:
          self.coors = []
          (_etype24, _size21) = iprot.readListBegin()
          for _i25 in xrange(_size21):
            _elem26 = Coor()
            _elem26.read(iprot)
            self.coors.append(_elem26)
          iprot.readListEnd()
        else:
          iprot.skip(ftype)
      else:
        iprot.skip(ftype)
      iprot.readFieldEnd()
    iprot.readStructEnd()

  def write(self, oprot):
    if oprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and self.thrift_spec is not None and fastbinary is not None:
      oprot.trans.write(fastbinary.encode_binary(self, (self.__class__, self.thrift_spec)))
      return
    oprot.writeStructBegin('LinkInfo')
    if self.link_id is not None:
      oprot.writeFieldBegin('link_id', TType.I64, 1)
      oprot.writeI64(self.link_id)
      oprot.writeFieldEnd()
    if self.road_class is not None:
      oprot.writeFieldBegin('road_class', TType.I32, 2)
      oprot.writeI32(self.road_class)
      oprot.writeFieldEnd()
    if self.formway is not None:
      oprot.writeFieldBegin('formway', TType.I32, 3)
      oprot.writeI32(self.formway)
      oprot.writeFieldEnd()
    if self.length is not None:
      oprot.writeFieldBegin('length', TType.I32, 4)
      oprot.writeI32(self.length)
      oprot.writeFieldEnd()
    if self.kmph is not None:
      oprot.writeFieldBegin('kmph', TType.DOUBLE, 5)
      oprot.writeDouble(self.kmph)
      oprot.writeFieldEnd()
    if self.coors is not None:
      oprot.writeFieldBegin('coors', TType.LIST, 6)
      oprot.writeListBegin(TType.STRUCT, len(self.coors))
      for iter27 in self.coors:
        iter27.write(oprot)
      oprot.writeListEnd()
      oprot.writeFieldEnd()
    oprot.writeFieldStop()
    oprot.writeStructEnd()

  def validate(self):
    if self.link_id is None:
      raise TProtocol.TProtocolException(message='Required field link_id is unset!')
    if self.road_class is None:
      raise TProtocol.TProtocolException(message='Required field road_class is unset!')
    if self.formway is None:
      raise TProtocol.TProtocolException(message='Required field formway is unset!')
    if self.length is None:
      raise TProtocol.TProtocolException(message='Required field length is unset!')
    if self.kmph is None:
      raise TProtocol.TProtocolException(message='Required field kmph is unset!')
    if self.coors is None:
      raise TProtocol.TProtocolException(message='Required field coors is unset!')
    return


  def __hash__(self):
    value = 17
    value = (value * 31) ^ hash(self.link_id)
    value = (value * 31) ^ hash(self.road_class)
    value = (value * 31) ^ hash(self.formway)
    value = (value * 31) ^ hash(self.length)
    value = (value * 31) ^ hash(self.kmph)
    value = (value * 31) ^ hash(self.coors)
    return value

  def __repr__(self):
    L = ['%s=%r' % (key, value)
      for key, value in self.__dict__.iteritems()]
    return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

  def __eq__(self, other):
    return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

  def __ne__(self, other):
    return not (self == other)

class MatchedGpsRoute:
  """
  Attributes:
   - link_list
   - gps_link_idx
  """

  thrift_spec = (
    None, # 0
    (1, TType.LIST, 'link_list', (TType.I64,None), None, ), # 1
    (2, TType.LIST, 'gps_link_idx', (TType.I32,None), None, ), # 2
  )

  def __init__(self, link_list=None, gps_link_idx=None,):
    self.link_list = link_list
    self.gps_link_idx = gps_link_idx

  def read(self, iprot):
    if iprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and isinstance(iprot.trans, TTransport.CReadableTransport) and self.thrift_spec is not None and fastbinary is not None:
      fastbinary.decode_binary(self, iprot.trans, (self.__class__, self.thrift_spec))
      return
    iprot.readStructBegin()
    while True:
      (fname, ftype, fid) = iprot.readFieldBegin()
      if ftype == TType.STOP:
        break
      if fid == 1:
        if ftype == TType.LIST:
          self.link_list = []
          (_etype31, _size28) = iprot.readListBegin()
          for _i32 in xrange(_size28):
            _elem33 = iprot.readI64()
            self.link_list.append(_elem33)
          iprot.readListEnd()
        else:
          iprot.skip(ftype)
      elif fid == 2:
        if ftype == TType.LIST:
          self.gps_link_idx = []
          (_etype37, _size34) = iprot.readListBegin()
          for _i38 in xrange(_size34):
            _elem39 = iprot.readI32()
            self.gps_link_idx.append(_elem39)
          iprot.readListEnd()
        else:
          iprot.skip(ftype)
      else:
        iprot.skip(ftype)
      iprot.readFieldEnd()
    iprot.readStructEnd()

  def write(self, oprot):
    if oprot.__class__ == TBinaryProtocol.TBinaryProtocolAccelerated and self.thrift_spec is not None and fastbinary is not None:
      oprot.trans.write(fastbinary.encode_binary(self, (self.__class__, self.thrift_spec)))
      return
    oprot.writeStructBegin('MatchedGpsRoute')
    if self.link_list is not None:
      oprot.writeFieldBegin('link_list', TType.LIST, 1)
      oprot.writeListBegin(TType.I64, len(self.link_list))
      for iter40 in self.link_list:
        oprot.writeI64(iter40)
      oprot.writeListEnd()
      oprot.writeFieldEnd()
    if self.gps_link_idx is not None:
      oprot.writeFieldBegin('gps_link_idx', TType.LIST, 2)
      oprot.writeListBegin(TType.I32, len(self.gps_link_idx))
      for iter41 in self.gps_link_idx:
        oprot.writeI32(iter41)
      oprot.writeListEnd()
      oprot.writeFieldEnd()
    oprot.writeFieldStop()
    oprot.writeStructEnd()

  def validate(self):
    if self.link_list is None:
      raise TProtocol.TProtocolException(message='Required field link_list is unset!')
    if self.gps_link_idx is None:
      raise TProtocol.TProtocolException(message='Required field gps_link_idx is unset!')
    return


  def __hash__(self):
    value = 17
    value = (value * 31) ^ hash(self.link_list)
    value = (value * 31) ^ hash(self.gps_link_idx)
    return value

  def __repr__(self):
    L = ['%s=%r' % (key, value)
      for key, value in self.__dict__.iteritems()]
    return '%s(%s)' % (self.__class__.__name__, ', '.join(L))

  def __eq__(self, other):
    return isinstance(other, self.__class__) and self.__dict__ == other.__dict__

  def __ne__(self, other):
    return not (self == other)
