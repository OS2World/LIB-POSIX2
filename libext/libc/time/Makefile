# $Header: /cvsroot/posix2/posix2/libext/libc/time/Makefile,v 1.6 2000/11/25 16:33:36 arnd Exp $

INC 	= ../../include/
MKINC 	= ../../../usr/share/mk/

SRCS    = timezone.c itimer.c

HEADERS = $(INC)sys/types.h $(INC)sys/time.h $(INC)sys/itimer.h \
 	 $(INC)sys/unistd.h $(INC)sys/unistdx.h $(INC)sys/errnox.h \
	 $(INC)tzfile.h

include ../../Ext.mk

DEFADD+= -D_THREAD_EXPERIMENTAL

# Dependencies
itimer.o: $(INC)sys/signal.h

