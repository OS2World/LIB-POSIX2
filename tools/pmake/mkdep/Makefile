#$Header: /cvsroot/posix2/posix2/tools/pmake/mkdep/Makefile,v 1.2 2000/05/17 08:10:17 veit Exp $
#	$OpenBSD: Makefile,v 1.5 1997/09/21 11:50:02 deraadt Exp $

MAN=	mkdep.1

beforeinstall:
	${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m ${BINMODE} \
	    ${.CURDIR}/mkdep.gcc.sh ${DESTDIR}/usr/bin/mkdep

.include <bsd.prog.mk>
