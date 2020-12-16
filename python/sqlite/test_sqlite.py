#! coding=utf-8

import sqlite3
from io import BytesIO

def test1():
    '''
    创建一个数据库，并落盘
    '''
    c = sqlite3.connect('school_manage.db')
    c.execute('create table tbl_student (name string, age int);')
    c.execute('insert into tbl_student values (\'zhangsan\', 10);')
    c.commit()
    c.close()

def test2():
    '''
    复制一个已存在的数据库到内存中
    '''
    c = sqlite3.connect('school_manage.db')
    tempfile = BytesIO()
    for line in c.iterdump():
        # line 有的是 type<'str'>，有的是 type<'unicode'>
        line = line.encode('ascii', 'ignore')
        tempfile.write('%s' % line)
    c.close()
    tempfile.seek(0)
    
    c = sqlite3.connect(":memory:")
    c.cursor().executescript(tempfile.read())
    c.commit()
    c.row_factory = sqlite3.Row
    d = c.execute('select * from tbl_student')
    print d.fetchall()

#test1()
test2()
