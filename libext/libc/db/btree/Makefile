# $Header 07:44:52 veit Exp $

INC = ../../../include/

DEFINES  = -D__DBINTERFACE_PRIVATE -D__ST_MT_ERRNO__ -Zmt 

AR     = ar rc
EMXOMF = emxomf
RANLIB = ar s
RM     = rm -f

.SUFFIXES:
.SUFFIXES: .a .aux .c .cpp .cxx .C .def .dll .dvi .exe .hlp .inf .ipf \
           .lib .log .map .o .obj .rc .res .tex .toc
.PHONY: clean default distclean

CC = gcc
DEBUG    = -g -O2 -mpentium -D_DEBUG -DDEBUG

WFLAGS   = -Wall -W -Wmissing-prototypes -Wshadow -Wcast-align \
   -Wpointer-arith -Winline -Wconversion -Wmissing-declarations  
INCLUDE  = -I. -I$(INC) -I$(X11ROOT)/XFree86/include
CFLAGS  = $(DEFINES) $(DEBUG) $(WFLAGS) $(INCLUDE)


SRCS =  bt_close.c bt_conv.c bt_debug.c bt_delete.c bt_get.c bt_open.c \
	bt_overflow.c bt_page.c bt_put.c bt_search.c bt_seq.c bt_split.c \
	bt_utils.c
 
OBJS = $(SRCS:.c=.o)

all: $(OBJS) 

# dependencies
$(OBJS): $(INC)db.h extern.h btree.h 

clean:
	$(RM) *.a *.lib *.o *.obj core a.out
