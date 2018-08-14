import StringIO

buf = "hello world.\nI'm xxx\n"

file_obj = StringIO.StringIO(buf)

print file_obj.readlines()

file_obj.close()
