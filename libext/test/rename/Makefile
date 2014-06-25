# $Header: /cvsroot/posix2/posix2/libext/test/rename/Makefile,v 1.3 2000/08/21 21:37:55 amai Exp $
# requires GNUmake    

# tools section
AR     = ar rc
AWK    = gawk
CC     = gcc
CP     = cp
DVIPS  = dvips
EMXEXP = emxexp
EMXIMP = emximp
EMXOMF = emxomf
IPFC   = ipfc
LATEX  = latex
LD     = gcc
LEX    = flex
LN     = cp
RANLIB = ar s
RC     = rc
RM     = rm -f
SED    = sed
TEX    = latex
YACC   = bison
CTAGS  = ctags

TESTAPPS = renametest.exe
EXTLIB = ../../cExt.a

# flags section
#For easy debugging do not use gcc '__builtin_' and inline functions:
#  -fno-builtin -fno-inline
DEBUG     = -g -O2 -mpentium -D_DEBUG -DDEBUG -DTHREAD_EXPERIMENTAL \
   -D_USE_HRTIMER_SYS -D_CPYFILE_FOR_SYMLINK -DDEBUG -fno-builtin -fno-inline

WFLAGS   = -Wall -W -Wmissing-prototypes -Wshadow -Wcast-align \
   -Wpointer-arith -Winline -Wmissing-declarations  \
   -Wstrict-prototypes -Wconversion
 
DEFINES  = -D__ST_MT_ERRNO__ -Zmt -DGNUFNS -fno-builtin
INCLUDE  = -I../include -I$(X11ROOT)/XFree86/include
CFLAGS  = $(DEFINES) $(DEBUG) $(WFLAGS) $(INCLUDE)

LIBRARY    = -L../.. -L$(X11ROOT)/XFree86/lib -lcExt
#-lsocket -lstdcpp
DEF          = ..\EXE.DEF
DLLDEFFILE = 
DLLFLAGS   = -Zomf -Zmtd -Zdll $(DLLDEFFILE) 
LDFLAGS    =  $(CFLAGS) -Zmtd -Zbin-files -Zbsd-signals \
              $(DEF) $(LIBRARY) 

.SUFFIXES:
.SUFFIXES: .a .aux .c .cpp .cxx .def .dll .dvi .exe .hlp .inf .ipf \
           .lib .log .map .o .obj .rc .res .tex .toc
.PHONY: clean default distclean

default: all

all: test

run:  test

test: $(TESTAPPS)

renametest.exe: renametest.c $(EXTLIB)
	$(CC) renametest.c -o $@ $(LDFLAGS)



#
# add dependencies here
#
# foo: bar
$(TESTAPPS): $(EXTLIB)

# Generic part: rules

%.a: %.def
	$(EMXIMP) -o $@ $<

%.c : %.l
	$(LEX) -o $@ $<

%.dll: %.lib %.def
	$(CC) -o $@ $< $(DLLFLAGS)

%.exe: %.obj
	$(CC) -Zomf -o $@ $^ $(LDFLAGS)

%.exe: %.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.lib: %.a
	$(EMXOMF) $<

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%.o : %.cxx
	$(CC) $(CFLAGS) -c $< -o $@

%.obj : %.c
	$(CC) $(CFLAGS) -Zomf -c $< -o $@

%.obj : %.cpp
	$(CC) $(CFLAGS) -Zomf -c $< -o $@

%.obj : %.cxx
	$(CC) $(CFLAGS) -Zomf -c $< -o $@

#
# not-programming related
#

%.dvi: %.tex
	$(TEX) $<

%.ps: %.dvi
	$(DVIPS) $<

#
# OS/2 specific targets
#
%.hlp: %.ipf
	$(IPFC) $<

%.inf: %.ipf
	$(IPFC) -i $<

%.res: %.rc
	$(RC) -r $<

#
# Common maintainance targets
#
clean:
	cd regex && $(RM) *.o *.obj *.res *.aux *.log *.toc core *.a  \
 *.lib dummy exp *.exe *.dll copy.dat *.tst *.tmp
	$(RM) *.o *.obj *.res *.aux *.log *.toc core *.a *.lib dummy exp \
 *.exe *.dll *.dat *.tst *.tmp 

veryclean: clean

install:

tags:
	$(CTAGS) -R *
