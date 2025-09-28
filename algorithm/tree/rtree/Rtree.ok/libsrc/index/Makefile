#!/bin/sh

# R-Tree Library Makefile
#
# Arguments:
#	CIMSG	- ci log message
#	CIREV	- ci revision level (optional)
#		(of the form -fx.x, to *force* a check in at x.x)

# R-Tree Specific Directories
LIB=		../../lib
RTINCL=		../../include


# Compiler Options
OPTIM=	-g
#OPTIM=	-O
CFLAGS=	$(OPTIM) -I$(RTINCL)
#CFLAGS=	$(OPTIM) -I$(RTINCL) -DNDEBUG
#CFLAGS=	$(OPTIM) -I$(RTINCL) -DRECT_LIMIT=200

# RCS Check In Command
CI=	/usr/new/ci

# Library Src 
SRC=	func.c \
	init.c \
	index.c \
	printstats.c \
	node.c \
	rectangle.c \
	split_l.c \
	page.c \
	getsltuples.c \
	clock.c \
	allocate.c \
	error.c

# Library Objs
OBJ=	func.o \
	init.o \
	index.o \
	printstats.o \
	node.o \
	rectangle.o \
	split_l.o \
	page.o \
	getsltuples.o \
	clock.o \
	allocate.o \
	error.o

HEADERS= $(RTINCL)/assert.h $(RTINCL)/macros.h $(RTINCL)/rtree.h \
	$(RTINCL)/split_l.h $(RTINCL)/timer.h $(RTINCL)/error.h

RTLIB=	$(LIB)/librtree.a

all:	$(RTLIB)

$(OBJ):	$(HEADERS)

$(RTLIB): $(OBJ)
	ar cr $(RTLIB) $(OBJ)
	ranlib $(RTLIB)

.c.o:	$(HEADERS)
	cc  -c $(CFLAGS) $*.c -lm


# ======================== Maintanence Targets =========================
# Remove previously made version
clean:
	rm -fr *.o *.bak $(LIB)/librtree.a

# Check in src & include files
ci:
	$(CI) -u -m"$(CIMSG)" $(CIREV) $(SRC) Makefile *.c &
