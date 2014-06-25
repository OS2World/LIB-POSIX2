# $Header: /cvsroot/posix2/posix2/libext/libc/regex/Makefile,v 1.8 2000/11/25 16:33:35 arnd Exp $

SRCS    = regcomp.c regerror.c regexec.c regfree.c cname.c

HEADERS = engine.c cclass.h cname.h regex2.h utils.h $(INC)regex.h

INC 	= ../../include/
MKINC 	= ../../../usr/share/mk/

include ../../Ext.mk

DEFADD+= -DPOSIX_MISTAKE 
