# $Header: /cvsroot/posix2/posix2/libext/libstcExt/Makefile,v 1.7 2000/09/11 00:35:29 arnd Exp $

# A library containing libExt modules that we link statically, always.

LIB= stcExt

SRCS= 	err.c getbsize.c 43re_exp.c# mmap.c
HEADERS=$(INC)err.h 

INC 	= ../include/
MKINC 	= ../../usr/share/mk/

include ../Ext.mk

## dependencies
43re_exp.o: $(INC)regexp.h regmagic.h
#mmap.o: $(INC)sys/p2mman.h $(INC)sys/mman.h