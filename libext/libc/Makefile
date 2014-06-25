# $Header: /cvsroot/posix2/posix2/libext/libc/Makefile,v 1.25 2000/11/25 16:33:35 arnd Exp $

# The comments below are from original BSD source and not always true now:

# All library objects contain sccsid strings by default; they may be
# excluded as a space-saving measure.  To produce a library that does
# not contain these strings, delete -DLIBC_SCCS and -DSYSLIBC_SCCS
# from CFLAGS below.  To remove these strings from just the system call
# stubs, remove just -DSYSLIBC_SCCS from CFLAGS.
#
# The NLS (message catalog) functions are always in libc.  To choose that
# strerror(), perror(), strsignal(), psignal(), etc. actually call the NLS
# functions, put -DNLS on the CFLAGS line below.
#
# The YP functions are always in libc. To choose that getpwent() and friends
# actually call the YP functions, put -DYP on the CFLAGS line below.
#

LIB = cExt

NLS=	C.msg Pig.msg de.msg es.msg fi.msg fr.msg nl.msg no.msg ru.msg sv.msg

SRCS = vis.c libgen.c ctype.c 

SUB = sub
SUBCLEAN = subclean
EXP = c.exp

# flags section

INC = ../include/
MKINC 	= ../../usr/share/mk/
 
SUBOBJS = stdiox/*.o stdlibx/*.o unistdx/*.o db/*.o db/btree/*.o db/hash/*.o \
 db/mpool/*.o db/recno/*.o regex/*.o nls/*.o stringx/*.o time/*.o net/*.o \
 rpc/*.o yp/*.o sys/*.o libm/*.o

include ../Ext.mk

# Has to be adapted to the used suffix(es)

sub:
	$(RM) *.a *.lib
	cd stdiox && $(MAKE) all
	cd stdlibx && $(MAKE) all
	cd unistdx && $(MAKE) all
	cd regex && $(MAKE) all
	cd net && $(MAKE) all
	cd nls && $(MAKE) all
	cd rpc && $(MAKE) all
	cd yp && $(MAKE) all
	cd stringx && $(MAKE) all
	cd db && $(MAKE) all
	cd time && $(MAKE) all
	cd sys && $(MAKE) all
	cd libm && $(MAKE) all

#
# Common maintainance targets
#
subclean:
	cd stdiox && $(MAKE) clean
	cd stdlibx && $(MAKE) clean
	cd unistdx && $(MAKE) clean
	cd regex && $(MAKE) clean
	cd rpc && $(MAKE) clean
	cd yp && $(MAKE) clean
	cd net && $(MAKE) clean
	cd nls && $(MAKE) clean
	cd stringx && $(MAKE) clean
	cd db && $(MAKE) clean
	cd time && $(MAKE) clean
	cd sys && $(MAKE) clean
	cd libm && $(MAKE) clean

install:


# DO NOT DELETE THIS LINE -- make  depend  depends  on it.

#
# add dependencies here
#
# foo: bar
vis.o: $(INC)vis.h
ctype.o: $(INC)ctype.h
