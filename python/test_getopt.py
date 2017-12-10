import sys
import getopt

if __name__ == '__main__':
	opts, args = getopt.getopt(sys.argv[1:], "ho:pc:", ["help", "output="])
	print "opts:", opts
	print "args:", args
	for name, value in opts:
		print name, value
