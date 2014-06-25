# $Header: /cvsroot/posix2/posix2/tools/rpc.bootparamd/Makefile,v 1.2 2000/11/17 19:17:34 arnd Exp $

#.include <bsd.own.mk>

#.if (${YP} == "yes")
#CFLAGS+=-DYP
#.endif

PROG=	rpc.bootparamd
SRCS=	bootparamd.c bootparam_prot_svc.c
#MAN=	bootparams.5 rpc.bootparamd.8
#MLINKS=	rpc.bootparamd.8 bootparamd.8

#DPADD=	${LIBRPCSVC}
LDADD=	-lrpcsvc

include ../tools.mk

bootparam_prot_svc.c: ${DESTDIR}/librpcsvc/bootparam_prot.x
	rm -f bootparam_prot.x
	$(CP) ${DESTDIR}/librpcsvc/bootparam_prot.x .
	rm -f bootparam_prot.h
	$(CP) ${DESTDIR}/include/rpcsvc/bootparam_prot.h .
	../rpcgen/rpcgen -mC -o $@ bootparam_prot.x

CLEANFILES += bootparam_prot_svc.c bootparam_prot.x bootparam_prot.h
WFLAGS = -Wall 
DEBUG += -D_USE_BSD