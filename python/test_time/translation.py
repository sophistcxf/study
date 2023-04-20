#! coding=utf-8

import datetime

'''
字符串时间转为 unix timestamp
'''
def str_to_unixtimestamp():
    str_tm = "2021-7-22 14:30:29"
    # dt is datatime.datetime 类型
    dt = datetime.datetime.strptime(str_tm, "%Y-%m-%d %H:%M:%S")
    print dt

    dt = dt + datetime.timedelta(0, 1, 0)
    print dt
    print dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second

str_to_unixtimestamp()
