#$Header: /cvsroot/posix2/posix2/tools/pmake/Makefile,v 1.11 2000/11/17 19:17:33 arnd Exp $

PROG=	pmake
SRCS=	arch.c buf.c compat.c cond.c dir.c for.c hash.c job.c main.c \
	make.c parse.c str.c suff.c targ.c var.c #util.c
LIB =	./lstlib/lst.a

include ../tools.mk

CFLAGS+= -Wno-unused 
MK   = ../pmake -m ../mk

all:: $(PROG).exe mkdp doc

./lstlib/lst.a:
	cd lstlib && $(MAKE) all

mkdp: mkdep/Makefile
	-(cd mkdep && $(MK))

doc: PSD.doc/Makefile
	-(cd PSD.doc && $(MK))
#
# Common maintainance targets
#
clean::
	$(RM) *.o *.obj *.res *.aux *.log *.toc core *.a dummy exp \
              *.exe *.dll copy.dat symlnk.tst mmaptest.tmp \
	      PSD.doc/paper.ps lstlib/*.o lstlib/core lstlib/*.a
 
veryclean:: clean
	-cd PSD.doc && $(MK)-m ../mk clean
	-cd mkdep && $(MK)-m ../mk clean
	cd lstlib && $(MAKE) clean
