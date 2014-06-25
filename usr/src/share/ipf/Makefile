#
#	$Id: Makefile,v 1.2 2000/08/18 18:52:10 amai Exp $
#
FILES=	example.* nat.* firewall.*
NOOBJ=	noobj

all clean cleandir depend lint tags:

install:
	install -d ${DESTDIR}${BINDIR}/ipf
	install -c -m 0444 ${FILES} ${DESTDIR}${BINDIR}/ipf

.include <bsd.prog.mk>
