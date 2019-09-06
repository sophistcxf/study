# coding=utf-8

class Series():
    def __init__(self):
        self.buf = []
    def append(self, v):
        self.buf.append(v)
    def __add__(self, v):
        for i,e in enumerate(self.buf):
            self.buf[i] += v

    '''
    支持 x in container
    '''
    def __contains__(self, v):
        return v in self.buf


    '''
    支持 abs
    '''
    def __abs__(self):
        return [ abs(v) for v in self.buf ]

    def to_string(self):
        return ','.join(map(str, self.buf))

s = Series()

s.append(5)
s.append(6)
print s.to_string()
s + 1
print s.to_string()

print 1 in s
print 6 in s

print abs(s)
