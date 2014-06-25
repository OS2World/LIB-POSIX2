# $Header: /cvsroot/posix2/posix2/tools/rpc.pcnfsd/Makefile,v 1.2 2000/11/17 19:17:34 arnd Exp $

PROG=	rpc.pcnfsd
MAN=	pcnfsd.8
MLINKS= pcnfsd.8 rpc.pcnfsd.8

DEFADD= -DUSER_CACHE -DWTMP #-DUSE_YP

#.if (${YP} == "yes")
#CFLAGS+=-DUSE_YP
#.endif

SRCS=	pcnfsd_v1.c pcnfsd_v2.c pcnfsd_misc.c \
	pcnfsd_cache.c pcnfsd_print.c pcnfsd_svc.c pcnfsd_xdr.c
HEADERS= pcnfsd.h

DPADD=	${LIBRPCSVC}
LDADD=	-lrpcsvc

include ../tools.mk

pcnfsd_svc.c: pcnfsd.x
	../rpcgen/rpcgen -C -s udp -s tcp -I ${.CURDIR}/pcnfsd.x -o $@

pcnfsd_xdr.c: pcnfsd.x
	../rpcgen/rpcgen -cC ./pcnfsd.x -o $@

pcnfsd.h: pcnfsd.x
	../rpcgen/rpcgen -hC ./pcnfsd.x -o $@

CLEANFILES += pcnfsd_svc.c pcnfsd_xdr.c pcnfsd.h
CLEANFILES += clnt.pcnfsd pcnfsd_test.o pcnfsd_test.ln pcnfsd_clnt.*


