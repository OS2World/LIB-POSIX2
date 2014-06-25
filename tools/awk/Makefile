# $Header: /cvsroot/posix2/posix2/tools/awk/Makefile,v 1.2 2000/08/14 16:26:07 amai Exp $

PROG=	awk
LINKS=	nawk
SRC=	ytab.c lex.c b.c main.c parse.c proctab.c tran.c lib.c run.c
HEADERS= proto.h awk.h
LDADD=	-lm
DPADD=	${LIBM}
CLEANFILES+=proctab.c maketab.exe ytab.c ytab.h
#CFLAGS+=-I. -I${.CURDIR}
MLINKS=	awk.1 nawk.1

include ../tools.mk


ytab.c ytab.h: awkgram.y
	${YACC} -d ./awkgram.y
	mv y.tab.c ytab.c
	mv y.tab.h ytab.h

proctab.c: maketab.exe
	maketab.exe >proctab.c && $(RM) maketab.exe

maketab.exe: ytab.h maketab.c
	${CC} ${CFLAGS} ./maketab.c -o $@


