# $Id: Makefile,v 1.5 2000/10/11 14:48:57 arnd Exp $
#
# C compiler
CC=gcc

# flags section
DEBUG     = -g -O2 -mpentium -D_DEBUG -DDEBUG -DTHREAD_EXPERIMENTAL \
   -D_USE_HRTIMER_SYS -D_CPYFILE_FOR_SYMLINK -DDEBUG 

WFLAGS   = -Wall -W -Wmissing-prototypes -Wshadow -Wcast-align \
   -Wpointer-arith -Winline -Wmissing-declarations  \
   -Wstrict-prototypes -Wconversion
 
DEFINES  = -D__ST_MT_ERRNO__ -Zmt -DGNUFNS 
INCLUDE  = -I../../include -I$(X11ROOT)/XFree86/include
CFLAGS  = $(DEFINES) $(DEBUG) $(WFLAGS) $(INCLUDE)

LIBRARY    = -L../.. -lcExt #-lsocket -lstdcpp
DEF          = ../EXE.DEF
DLLDEFFILE = 
DLLFLAGS   = -Zomf -Zmtd -Zdll $(DLLDEFFILE) 
LDFLAGS    =  $(CFLAGS) -Zmtd -Zbin-files -Zbsd-signals $(DEBUG) \
   $(DEF) $(LIBRARY) 

.SUFFIXES:
.SUFFIXES: .a .aux .c .cpp .cxx .def .dll .dvi .exe .hlp .inf .ipf \
           .lib .log .map .o .obj .rc .res .tex .toc
.PHONY: clean default distclean

PROG=	re.exe
SRCS=	main.c split.c debug.c
NOMAN=

# Has to be adapted to the used suffix(es)
OBJS     = $(SRCS:.c=.o) 

TESTS=	tests

default: all

all: $(PROG)

run: $(PROG) regress

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

regress:
	re < ${TESTS}
	re -el < ${TESTS}
	re -er < ${TESTS}


clean:
	rm -f *.o *.obj *.res *.aux *.log *.toc core *.a *.lib dummy exp \
 *.exe *.dll *.dat *.tst *.tmp 
