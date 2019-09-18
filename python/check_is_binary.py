#coding=utf-8

'''
判断是否为二进制文件
参考：
https://stackoverflow.com/questions/898669/how-can-i-detect-if-a-file-is-binary-non-text-in-python
'''

print __doc__

'''
    bool('') 是 False
    bool('hello') 是 True
'''
textchars = bytearray({7,8,9,10,12,13,27} | set(range(0x20, 0x100)) - {0x7f})
is_binary_string = lambda bytes: bool(bytes.translate(None, textchars))

# 打印 True
print is_binary_string(open('/usr/bin/python', 'rb').read())
# 打印 False
print is_binary_string(open('/etc/passwd', 'rb').read())

'''
测试汉字
如果是 Unicode 编码，translate 忽略 deletechars 参数
'''
a = '我在马路边'
print ":".join("{:02x}".format(ord(c)) for c in a)  # 输出 e6:88:91:e5:9c:a8:e9:a9:ac:e8:b7:af:e8:be:b9
print is_binary_string(a)
