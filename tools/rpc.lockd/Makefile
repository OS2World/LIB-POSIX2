# $Header: /cvsroot/posix2/posix2/tools/rpc.lockd/Makefile,v 1.2 2000/11/17 19:17:34 arnd Exp $

PROG =	rpc.lockd
SRCS =	nlm_prot_svc.c lockd.c procs.c
MAN =	rpc.lockd.8

DPADD=	${LIBRPCSVC}
LDADD=	-lrpcsvc

DEFADD= -DSYSLOG

CLEANFILES= nlm_prot_svc.c nlm_prot.h

RPCSRC= ${DESTDIR}/librpcsvc/nlm_prot.x
RPCGEN= ../rpcgen/rpcgen -L -C

include ../tools.mk

nlm_prot_svc.c: ${RPCSRC} nlm_prot.h
	${RPCGEN} -m -o $@ ${RPCSRC}

nlm_prot.h: ${RPCSRC}
	${RPCGEN} -h -o $@ ${RPCSRC}

test: test.c
	$(CC) -o test.exe test.c -lrpcsvc -lcExt -lsocket

WFLAGS = -Wall