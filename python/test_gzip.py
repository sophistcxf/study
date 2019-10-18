#! -*- coding=utf-8 -*-

import gzip
import StringIO

def test_gzip():
    con = "hello world"

    # 压缩为文件
    gzipper = gzip.GzipFile(filename='hello.gz', mode="wb")
    gzipper.write(con)
    gzipper.close()

    # 压缩为 StringIO
    s = StringIO.StringIO()
    gzipper = gzip.GzipFile(mode="wb", fileobj=s)
    gzipper.write(con)
    # 一定要 close，不然 StringIO 中的内容可能不全
    gzipper.close()
    f = open('hello2.gz', 'wb')
    f.write(s.getvalue())
    f.close()

    # 从 StringIO 中读压缩后的数据
    s.seek(0)
    print "read from StringIO: ",
    gzipper_2 = gzip.GzipFile(mode="rb", fileobj=s)
    data = gzipper_2.read()
    print data

    # 从文件中读
    print "read from file: ",
    f = open('hello2.gz', 'rb')
    s = StringIO.StringIO(f.read())
    f.close()
    gzipper_2 = gzip.GzipFile(mode="rb", fileobj=s)
    data = gzipper_2.read()
    print data


if __name__ == "__main__":
    test_gzip()
