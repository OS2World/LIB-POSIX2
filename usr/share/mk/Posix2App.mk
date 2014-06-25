# $Header: /cvsroot/posix2/posix2/usr/share/mk/Posix2App.mk,v 1.8 2000/11/17 19:17:34 arnd Exp $

##Add name of program and modules here
#Example:
#PROG=	foo
#SRC=	foo.c bar.c

DEFINES = -D__ST_MT_ERRNO__ -Zmt -Dlint -D_CPYFILE_FOR_SYMLINK $(DEFADD)
WFLAGS  = -Wall -W -Wcast-align -Wpointer-arith -Winline \
		-Wmissing-declarations -Wstrict-prototypes \
	   -Wconversion -Wno-char-subscripts 
CFLAGS  = $(DEFINES) $(DEBUG) $(WFLAGS) $(INCLUDE)
LDFLAGS =  $(DEF) -Zmtd -Zbsd-signals $(LDADD) 

##############
#GENERIC PART#
##############

# targets section

.SUFFIXES:
.SUFFIXES: .a .aux .c .cpp .cxx .def .dll .dvi .exe .hlp .inf .ipf \
           .lib .log .map .o .obj .rc .res .tex .toc
.PHONY: clean default distclean

OBJS     = $(SRC:.c=.o) $(SRCS:.c=.o) $(ASMOBJS)

all:: $(PROG).exe $(LINKS)

$(PROG).exe:: $(SYSLIB) $(LIB) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIB) $(LIBRARY)

$(LINKS):: $(PROG).exe
	$(CP) $(PROG).exe $@.exe

#
# Common maintainance targets
#
clean::
	$(RM) *.o *.obj *.res *.aux *.log *.toc core *.a dummy exp \
              *.a *.lib *.exe *.dll a.out $(OBJS) $(CLEANFILES)
 
veryclean:: clean


#Dependencies:

$(OBJS): $(SRC) $(HEADERS)
