# $Header: /cvsroot/posix2/posix2/libext/libc/stdlibx/Makefile,v 1.18 2000/11/26 16:43:01 arnd Exp $

SRCS = rand48.c random.c heapsort.c radixsort.c rev.c daemon.c a64l.c \
 quad.c getenv.c getsubopt.c merge.c valloc.c getcap.c arc4random.c 

HEADERS = $(INC)stdlib.h $(INC)sys/errnox.h 
	
INC 	= ../../include/
MKINC 	= ../../../usr/share/mk/

include ../../Ext.mk

## dependencies
rev.o: ../../rev.h
