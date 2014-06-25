# $Header: /cvsroot/posix2/posix2/tools/ed/Makefile,v 1.5 2000/08/14 16:26:08 amai Exp $

PROG= ed
SRCS=	 buf.c cbc.c glbl.c io.c main.c re.c sub.c undo.c

LINKS=  red
MLINKS= ed.1 red.1

include ../tools.mk

CFLAGS+=-DBACKWARDS # -DDES # libdes not yet ported
