import StringIO

def read():
    buf = "hello world.\nI'm xxx\n"

    file_obj = StringIO.StringIO(buf)

    print file_obj.readlines()

    file_obj.close()

    help(file_obj)

def write():
    file_obj = StringIO.StringIO()
    file_obj.write('hello world')
    r = file_obj.getvalue()
    print r
    file_obj.close()


#read()
write()
