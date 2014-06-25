#	from: @(#)Makefile	6.13 (Berkeley) 3/20/91
#	$Header: /cvsroot/posix2/posix2/usr/src/share/tmac/Makefile,v 1.2 2000/08/18 18:52:11 amai Exp $

BINDIR= /usr/share
FILES=	tmac.andoc
LINKS=	tmac.an
NOOBJ=	noobj

SUBDIR=	mdoc

# old macros not currently installed...
OFILES=	tmac.an.v6compat tmac.an6n tmac.an6t tmac.a tmac.cp tmac.doc.old

all clean cleandir depend lint tags:

realinstall:
	install -c -o ${BINOWN} -g ${BINGRP} -m 444 ${FILES} \
		${DESTDIR}${BINDIR}/tmac

include ../mk/Posix2App.mk
