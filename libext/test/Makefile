# $Header: /cvsroot/posix2/posix2/libext/test/Makefile,v 1.17 2000/10/11 14:48:56 arnd Exp $
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

TESTAPPS = itimertest.exe testusleep.exe testcpfile.exe dltest.exe \
	 test_poll.exe dltest.exe mmaptest.exe statxtest.exe shmsts.exe \
	 testshm2.exe popentst.exe mtst.exe round.exe except.exe dtestvec.exe \
	 fltestl.exe tstbitstring.exe testshm.exe stat.exe msgserver.exe \
	 msgclient.exe 
#	 clntshm.exe forkshm.exe xtestshm.exe

EXTLIB = ../cExt.a

PROG=	tstbitstring.exe

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

LIBRARY    = -L.. -L$(X11ROOT)/XFree86/lib -lcExt
#-lsocket -lstdcpp
DEF          = EXE.DEF
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

sub:
	cd regex && $(MAKE) all
	cd rename && $(MAKE) all

subrun:
	cd regex && $(MAKE) run
	cd rename && $(MAKE) run

lib:
	cd .. && $(MAKE) cExt.a

run: subrun  test regress
	$(RM) core copy.dat *.tst dummy
	-mmaptest
	-testshm 2>shm.tst
	-testshm2 2>shm2.tst
	itimertest 999
	-test_poll
	-dltest
	testusleep
	testcpfile testcpfile.exe copy.dat
	cmp testcpfile.exe copy.dat
	cmp copy.dat symlnk.tst
	-testcpfile testcpfile.exe copy.dat
	statxtest
	dtestvec
	round 
	-fltestl
	mtst
	tstbitstring
	except 0000
	popentst
	$(RM) copy.dat 

test: sub $(TESTAPPS)


itimertest.exe: itimertest.c $(EXTLIB) 
	$(CC) itimertest.c -o $@ $(LDFLAGS)

testusleep.exe: testusleep.c $(EXTLIB) 
	$(CC) testusleep.c -o $@ $(LDFLAGS)

testcpfile.exe: testcpfile.c $(EXTLIB) 
	$(CC) testcpfile.c -o $@ $(LDFLAGS)

test_poll.exe: test_poll.c $(EXTLIB) 
	$(CC) test_poll.c -o $@ $(LDFLAGS)  

dltest.exe: dt.c $(EXTLIB) sam.dll
	$(CC) dt.c -o $@ $(LDFLAGS)

mmaptest.exe: mmaptest.c $(EXTLIB) 
	$(CC) mmaptest.c -o $@ $(LDFLAGS)

statxtest.exe: statxtest.c $(EXTLIB) 
	$(CC) statxtest.c -o $@ $(LDFLAGS)

shmsts.exe: shmsts.c $(EXTLIB) 
	$(CC) shmsts.c -o $@ $(LDFLAGS)

testshm.exe: testshm.c $(EXTLIB) 
	$(CC) testshm.c -o $@ $(LDFLAGS)

testshm2.exe: testshm2.c $(EXTLIB) 
	$(CC) testshm2.c -o $@ $(LDFLAGS)

popentst.exe: popentst.c $(EXTLIB) 
	$(CC) popentst.c -o $@ $(LDFLAGS)

fltestl.exe: fltestl.c $(EXTLIB) 
	$(CC) fltestl.c -o $@ $(LDFLAGS)

mtst.exe: mtst.c $(EXTLIB) 
	$(CC) mtst.c -o $@ $(LDFLAGS)

dtestvec.exe: dtestvec.c $(EXTLIB) 
	$(CC) dtestvec.c -o $@ $(LDFLAGS)

renametest.exe: renametest.c $(EXTLIB) 
	$(CC) renametest.c -o $@ $(LDFLAGS)

round.exe: round.c $(EXTLIB) 
	$(CC) round.c -o $@ $(LDFLAGS)

stat.exe: stat.c $(EXTLIB) 
	$(CC) stat.c -o $@ $(LDFLAGS)

except.exe: except.c $(EXTLIB) 
	$(CC) except.c -o $@ $(LDFLAGS)

tstbitstring.exe: tstbitstring.c $(EXTLIB) 
	$(CC) tstbitstring.c -o $@ $(LDFLAGS)


sam.dll: sam.obj sam.def
	$(CC) $^ -o $@ -s -Zdll -Zno-rte -Zomf
#	$(CC) $^ -o $@ -Zdll -Zomf # you may need this for more complex examples

msgclient.exe: msgclient.C msge.h
	$(CXX) msgclient.C -o $@ $(LDFLAGS) -lstdcpp

msgserver.exe: msgserver.C msge.h
	$(CXX) msgserver.C -o $@ $(LDFLAGS) -lstdcpp

regress:	test-8 test-27 test-32 test-49 test-64 test-67

test-8: ${PROG}
	${PROG} 8 > 8.tst
	diff 8.tst ./good/8

test-27: ${PROG}
	${PROG} 27 > 27.tst
	diff 27.tst ./good/27

test-32: ${PROG}
	${PROG} 32 > 32.tst
	diff 32.tst ./good/32

test-49: ${PROG}
	${PROG} 49 > 49.tst
	diff 49.tst ./good/49

test-64: ${PROG}
	${PROG} 64 > 64.tst
	diff 64.tst ./good/64

test-67: ${PROG}
	${PROG} 67 > 67.tst
	diff 67.tst ./good/67

#
# add dependencies here
#
# foo: bar
$(TESTAPPS): $(EXTLIB)
dt.o: ../include/dlfcn.h dt.c
statxtest.o: ../include/sys/statx.h statxtest.c
testcpfile.o: ../include/sys/unistdx.h testcpfile.c
test_poll.o: ../include/sys/unistdx.h test_poll.c
mmaptest.o: ../include/sys/unistdx.h mmaptest.c
testusleep.o: ../include/sys/unistdx.h testusleep.c
tstbitstring.o: ../include/bitstring.h

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
