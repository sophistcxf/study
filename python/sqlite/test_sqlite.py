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

def test3():
    '''
    合并两个数据库
    '''
    fn1 = 'school_manage.db'
    fn2 = 'school_manage_bk.db'
    c = sqlite3.connect(fn1)
    c.execute('attach \'%s\' as db' % fn2)
    for row in c.execute("SELECT * FROM db.sqlite_master WHERE type='table'"):
        combine = 'insert into %s select * from db.%s' % (row[1], row[1])
        c.execute(combine)
    c.commit()
    c.execute('detach database db')

#test1()
#test2()
test3()
