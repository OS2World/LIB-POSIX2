#	$OpenBSD: Makefile,v 1.1 1996/11/11 05:06:33 downsj Exp $

PROG=	compile_et
SRCS=	compile_et.c error_table.y #et_lex.lex.l
DPADD=	${LIBL}
LDADD=	-ll
CLEANFILES=error_table.c y.tab.h et_lex.lex.c

beforedepend:	et_lex.lex.c
error_table.o:	et_lex.lex.c

include ../tools.mk

CFLAGS+= -I../../libext/libcom_err
