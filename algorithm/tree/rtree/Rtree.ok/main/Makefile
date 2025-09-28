#!/bin/sh

# Main Program/Application Makefile
#
# Arguments:
#	MACHINE - machine type
#	CIMSG	- ci log message (strongly suggested)
#	CIREV	- ci revision level (optional)
#		(of the form -fx.x)

# R-Tree Specific Directories
BIN=		../bin
LIB=		../lib
RTINCL=		../include


CFLAGS=	-g -I$(RTINCL)
#CFLAGS=		-O -I$(RTINCL)
LDFLAGS=	-L$(LIB) -g
#LDFLAGS=	-L$(LIB) -s
CI=	/usr/new/ci

#RTLIB=		$(LIB)/rtlib.a
SUBLEAFLIB=	$(LIB)/libsubleaf.a

SRC=	pack_main.c \
	insert_main.c \
	delete_main.c \
	search_main.c \
	print_main.c

OBJ=	pack_main.o \
	insert_main.o \
	delete_main.o \
	search_main.o \
	print_main.o

all:	pack insert search delete print

pack:	pack_main.o
	cc $(LDFLAGS) pack_main.o -lrtree -lsubleaf -o $(BIN)/rload  -lc -lm

insert:	insert_main.o
	cc $(LDFLAGS) insert_main.o -lrtree -lsubleaf -o $(BIN)/rinsert  -lc -lm

search:	search_main.o
	cc $(LDFLAGS) search_main.o -lrtree -lsubleaf  -o $(BIN)/rsearch -lc -lm

delete:	delete_main.o
	cc $(LDFLAGS) delete_main.o -lrtree -lsubleaf -o $(BIN)/rdelete -lc -lm

print:	print_main.o
	cc $(LDFLAGS) print_main.o -lrtree -lsubleaf  -o $(BIN)/rprint -lc -lm

# ======================== Maintanence Targets =========================
# Remove previously made version
clean:
	rm -fr *.o *.bak $(BIN)/r*

# Check in src & include files
ci:
	$(CI) -m"$(CIMSG)" $(CIREV) -u $(SRC) Makefile &

