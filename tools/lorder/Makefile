#	$Header file,v 1.3 1997/09/21 11:49:44 deraadt Exp $

MAN=	lorder.1

SCRIPT=	lorder.sh

realinstall:
	${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m ${BINMODE} \
	    ${.CURDIR}/${SCRIPT} ${DESTDIR}${BINDIR}/lorder

include ../tools.mk
