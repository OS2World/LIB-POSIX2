LIB = lst.a

SRCS=	lstAppend.c lstAtEnd.c lstAtFront.c lstClose.c lstConcat.c \
	lstDatum.c lstDeQueue.c lstDestroy.c lstDupl.c lstEnQueue.c \
	lstFind.c lstFindFrom.c lstFirst.c lstForEach.c lstForEachFrom.c \
	lstInit.c lstInsert.c lstIsAtEnd.c lstIsEmpty.c lstLast.c \
	lstMember.c lstNext.c lstOpen.c lstRemove.c lstReplace.c lstSucc.c

OBJ=lstAppend.o lstDupl.o lstInit.o lstOpen.o lstAtEnd.o lstEnQueue.o \
    lstInsert.o lstAtFront.o lstIsAtEnd.o lstClose.o lstFind.o lstIsEmpty.o \
    lstRemove.o lstConcat.o lstFindFrom.o lstLast.o lstReplace.o lstFirst.o \
    lstDatum.o lstForEach.o lstMember.o lstSucc.o lstDeQueue.o \
    lstForEachFrom.o lstDestroy.o lstNext.o 


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

DEBUG     = -g -O2 -mpentium -D_DEBUG -DDEBUG -DTHREAD_EXPERIMENTAL \
   -D_USE_HRTIMER_SYS -D_CPYFILE_FOR_SYMLINK -DDEBUG 

DEFINES  = -D__ST_MT_ERRNO__ -Zmt -DGNUFNS -Dlint

WFLAGS   = -Wall -W -Wmissing-prototypes -Wshadow -Wcast-align \
   -Wpointer-arith -Winline -Wmissing-declarations  \
   -Wstrict-prototypes -Wconversion -Wno-char-subscripts 
 
INCLUDE  = -I./.. -I../../posix2/libext/include 

CFLAGS  = $(DEFINES) $(DEBUG) $(WFLAGS) $(INCLUDE)


all: ${LIB} 

${LIB}: ${OBJ}
	$(AR) ${LIB} ${OBJ}
	$(RANLIB) ${LIB} 

clean:
	$(RM) *.o *.obj *.a *.lib core core.*