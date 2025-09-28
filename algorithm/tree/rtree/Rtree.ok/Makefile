# R-Tree Maintenance/Distribution Makefile

# The following two variables must be changed manually, for now
MACH=	mips

DISTHOST=	dias sparti delphi
DIRS=		libsrc \
 		main \

# Defalt to make
all:

# Options to pass to main & libs
all ci clean optimized:	lib bin
	for subdir in $(DIRS);	do \
		( cd $$subdir; echo "making in `pwd`"; make $@ MACHINE=${MACH} ); \
	done


# Remote distribution target(s)
rdist:
	rdist
sparti delphi dias:
	rdist -m $@

# Make sure these dirs exist
lib:
	-mkdir lib

bin:
	-mkdir bin
