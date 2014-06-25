# $Header: /cvsroot/posix2/posix2/libext/libc/net/Makefile,v 1.2 2000/11/25 19:15:57 arnd Exp $

INC 	= ../../include/
MKINC 	= ../../../usr/share/mk/

SRCS    = inet_addr.c getdomainname.c
HEADERS = $(INC)sys/types.h $(INC)sys/param.h $(INC)netinet/in.h $(INC)arpa/inet.h 

include ../../Ext.mk

# Dependencies
getdomainname.o: $(INC)sys/unistdx.h
