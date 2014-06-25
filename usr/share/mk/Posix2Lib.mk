# $Header: /cvsroot/posix2/posix2/usr/share/mk/Posix2Lib.mk,v 1.7 2000/11/25 16:33:36 arnd Exp $

DEFINES = -D__ST_MT_ERRNO__ -Zmtd $(DEFADD) 
WFLAGS  = -Wall -W -Wcast-align -Wpointer-arith -Winline \
	  -Wmissing-declarations  -Wstrict-prototypes \
	  -Wconversion -Wno-char-subscripts -Wshadow 
INCLUDE  = -I. -I$(INC)
CPPFLAGS += $(DEFINES) $(INCLUDE)
CFLAGS   = $(DEBUG) $(WFLAGS)

##############
#GENERIC PART#
##############

# targets section

.SUFFIXES:
.SUFFIXES: .a .aux .c .cpp .cxx .def .dll .dvi .exe .hlp .inf .ipf \
           .lib .log .map .o .obj .rc .res .tex .toc .S .s .x .h
.PHONY: all clean default distclean

OBJS = $(SRC:.c=.o) $(SRCS:.c=.o) $(SUBOBJS) $(ASMOBJS)

all:: $(LIB) $(OBJS) $(LINKS) $(EXP)

$(LIB): $(SUB) $(LIB).a $(LIB).lib
	$(CP) *.a *.lib ..

$(LIB).a: $(OBJS)
	$(RM) $@
	$(AR) $@ $^
	
$(LIB).lib: $(LIB).a
	$(EMXOMF) -o $@ $^

$(EXP): $(LIB).a 
	$(EMXEXP) $(LIB).a > $@

$(LINKS): $(LIB).a $(LIB).lib
	$(CP) $(LIB).a $@.a
	$(CP) $(LIB).lib $@.lib


# Generic part: rules
#
.S.o:
	$(CC) -c $(CPPFLAGS) $(ASFLAGS) -o $@ $<

.x.c:
	rpcgen -C -c $< -o $@ 

.x.h:
	rpcgen -C -h $< -o $@ 

# Common maintainance targets
#
tags::
	$(CTAGS) *.c

clean::
	$(RM) *.o *.obj *.res *.aux *.log *.toc core *.a dummy *exp *.bak \
 *.dat *.def *.a *.lib a.out $(OBJS) $(CLEANFILES) $(LIB).a $(LIB).lib 
 
veryclean:: $(SUBCLEAN)
	$(RM) *.exe *.dll tags *exp *.o *.obj *.res *.aux *.log *.bak \
 *.toc core *.a dummy exp *.dat *.def *.a *.lib a.out $(OBJS) \
 $(CLEANFILES) $(LIB).a $(LIB).lib 


#Dependencies:
$(OBJS): $(HDRS) $(HEADERS)
