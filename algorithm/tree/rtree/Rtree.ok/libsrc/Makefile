#directories to make in.
DIRS=	index \
	subleaf

#default thing to make
all:

#options. passed on to the next level down
all lint clobber clean ci:
	for subdir in $(DIRS); do \
		( cd $$subdir; echo "making in `pwd`"; make $@ ); \
	done

#optimized make
optimized:
	for subdir in $(DIRS); do \
		( cd $$subdir; echo "making in `pwd`"; make "OPTIM= -O"); \
	done

#profiled make
profiled:
	for subdir in $(DIRS); do \
		( cd $$subdir; echo "making in `pwd`"; make "OPTIM= -p"); \
	done

#gprof profiled make
gprofiled:
	for subdir in $(DIRS); do \
		( cd $$subdir; echo "making in `pwd`"; make "OPTIM= -pg"); \
	done
