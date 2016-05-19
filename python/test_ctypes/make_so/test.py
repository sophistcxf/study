from ctypes import *


# test getResult
# =================================================================================
class Result(Structure):
    _fields_ = [("name", c_char * 20), ("age", c_int)]

mylib = cdll.LoadLibrary("./mylib.so")

mylib.getResult.restype = POINTER(Result)
rlt = mylib.getResult("abc", 10)
print "%s.%d" % (rlt.contents.name, rlt.contents.age) 
# =================================================================================


# test outputString 
# =================================================================================
class OutInt(Structure):
    _fields_ = [('v', c_int)]
out_len = OutInt()
line_num = 50000 
mylib.outputString.restype = c_char_p
handle = mylib.createDataHandler()
data = mylib.outputString(handle, line_num, byref(out_len))
rst = string_at(data)
for i in rst.strip().split('\n'):
    print i
mylib.destroyDataHandler(handle)
# =================================================================================
