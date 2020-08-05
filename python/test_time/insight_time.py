#! coding=utf-8

import time

def test_time():
    print time.localtime()
    print time.time()

def test_struct_time():
    '''
    time 只有 struct_time 一个类
    time.struct_time 是一个九元组
    tm_year
    tm_mon
    tm_mday
    tm_wday
    tm_yday
    tm_hour
    tm_min
    tm_sec
    tm_isdst
    '''
    st = time.localtime()
    print st

def test_strftime():
    st = time.localtime()
    print time.strftime('%Y-%m-%d %H:%M:%S', st)

def test_strptime():
    print time.strptime('2020-07-01 10:00:00', '%H:%m:%d %H:%M:%S')

def test_mktime():
    st = time.localtime()
    print time.mktime(st)

def test_gmtime():
    st = time.localtime()
    unix_time = time.mktime(st)
    print time.gmtime(unix_time)

def test_asctime():
    st = time.localtime()
    print time.asctime(st)

def test_clock():
    print time.clock()

def test_ctime():
    print time.ctime()

#test_struct_time()
#test_strftime()
#test_mktime()
#test_asctime()
#test_clock()
#test_gmtime()
#test_time()
test_ctime()
