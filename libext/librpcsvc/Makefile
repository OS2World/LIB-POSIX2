# $Header: /cvsroot/posix2/posix2/libext/librpcsvc/Makefile,v 1.4 2000/11/26 16:43:01 arnd Exp $

INC 	= ../include/
MKINC 	= ../../usr/share/mk/


RPCSRCS=	bootparam_prot.x klm_prot.x mount.x nfs_prot.x\
		nlm_prot.x rex.x rnusers.x rusers.x rquota.x rstat.x rwall.x\
		sm_inter.x spray.x #yp.x yppasswd.x
SRCS=		${RPCSRCS:.x=.c}
RPCHDRS=        ${RPCSRCS:.x=.h}
CLEANFILES+=	${SRCS} ${RPCHDRS} rpcgen.exe *.c *.h

LIB=		rpcsvc
# NOMAN=
# NOPIC=

all::		rpcgen ${RPCHDRS} INSTHDRS

beforedepend:	${HDRS}

${RPCHDRS}:	rpcgen.exe
$(SRCS): 	${RPCHDRS}  $(INC)rpc/svc.h $(INC)rpc/clnt.h $(INC)rpc/xdr.h \
		$(INC)rpc/auth.h $(INC)netinet/in.h $(INC)rpc/rpc.h

INSTHDRS:	${RPCHDRS}
	$(CP) ${RPCHDRS} $(INC)rpcsvc/

#includes:	${HDRS}
#	${INSTALL} -d -o ${BINOWN} -g ${BINGRP} -m 755 \
#		${DESTDIR}/usr/include/rpcsvc
#	@echo installing rpc .h and .x files
#	@for i in $(HDRS); do \
#	    cmp -s $$i ${DESTDIR}/usr/include/rpcsvc/$$i || \
#		{ j="${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 444 $$i \
#		${DESTDIR}/usr/include/rpcsvc"; \
#		echo $$j; $$j; } \
#	done
#	@cd ${.CURDIR}; for i in $(RPCSRCS); do \
#	    cmp -s $(.CURDIR)/$$i ${DESTDIR}/usr/include/rpcsvc/$$i || \
#		{ j="${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 444 $$i \
#		${DESTDIR}/usr/include/rpcsvc"; \
#		echo $$j; $$j; } \
#	done

# new suffixes have to go afterwards, because bsd.lib.mk clears them
#.include <bsd.lib.mk>
#.SUFFIXES: .x .c .h

rpcgen: rpcgen.exe
	$(CP) ../../tools/rpcgen/rpcgen.exe .

rpcgen.exe: 
	cd .. && cd .. && cd tools && cd rpcgen && $(MAKE) all

include ../Ext.mk

WFLAGS += -Wno-unused -Wno-switch -Wno-strict-prototypes -Wno-conversion
