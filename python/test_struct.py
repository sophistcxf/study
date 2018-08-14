import struct
import sys

def test1():
    def impl(format):
        packed = struct.pack(format, 1, 2, 3, 20, 'a', "cxf")
        print map(hex, map(ord, packed))
        unpacked = struct.unpack(format, packed)
        print unpacked
        print struct.calcsize(format)

    print sys.byteorder

    print "byte order: big-endian, size: standard, alignment: none"
    impl('>hhllcs')

    print "byte order: little-endian, size: standard, alignment: none"
    impl('<hhllcs')

    print "byte order: native, size: native, alignment: native"
    impl('@hhllcs')

    print "byte order: native, size: native, alignment: none"
    impl('=hhllcs')

test1()
