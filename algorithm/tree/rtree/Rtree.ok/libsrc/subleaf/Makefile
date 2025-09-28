#definition of directories - use relative path names only:

#place to assemble library archives
LIBDIR= ../../lib
#place to assemble constructed executables
BINDIR= ../../bin
#place to find include files
INCDIR= ../../include


#various compiler/linker/etc options
OPTIM= -g
#OPTIM= -O
CFLAGS= -I$(INCDIR) $(OPTIM)

#RCS checkin command
CI= /usr/new/ci

#libraries to link to - do NOT use this as the address of libraries to
#create! That should be defined explicitly
LIBS=

SRC=	afileio.c \
	deletea.c \
	inserta.c \
	xgettuple.c \
	xnext_tid.c

OBJ=	afileio.o \
	deletea.o \
	inserta.o \
	xgettuple.o \
	xnext_tid.o


SUBLEAFLIB= $(LIBDIR)/libsubleaf.a

#this must be the default thing(s) to make
all: $(SUBLEAFLIB)

$(SUBLEAFLIB): $(OBJ)
	ar rcv $@ $(OBJ)
	ranlib $@

#clean up any by-products, .o files, etc.
clean:
	rm -f *.bak $(OBJ)

#clobber cleans all by-products as well as the final product
clobber: clean
	rm -f $(SUBLEAFLIB)

ci:
	$(CI) -t/dev/null -m" " -u $(SRC) Makefile &
