import sys
from optparse import OptionParser

if __name__ == "__main__":
    opt_parser = OptionParser()
    opt_parser.add_option('-f', '--file', type='string', dest='filename')

    opt, args = opt_parser.parse_args(sys.argv[1:])

    print type(opt)

    print opt
    print args
