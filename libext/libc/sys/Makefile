# $Header: /cvsroot/posix2/posix2/libext/libc/sys/Makefile,v 1.5 2000/11/25 16:33:36 arnd Exp $

INC 	= ../../include/
MKINC 	= ../../../usr/share/mk/

SRCS    = errnox.c mm.c resource.c signal.c stat.c stubs.c sync.c sysconf.c \
	  uname.c wait34.c #msg.c 

HEADERS = $(INC)sys/types.h $(INC)unistd.h $(INC)sys/unistd.h $(INC)sys/param.h \
	  $(INC)sys/unistdx.h $(INC)sys/errnox.h $(INC)sys/vlimit.h 

include ../../Ext.mk

DEFADD+= -D_THREAD_EXPERIMENTAL

# Dependencies
stat.o: $(INC)sys/stat.h $(INC)sys/statfs.h $(INC)sys/vfs.h $(INC)sys/mount.h 
wait34.o resource.o: $(INC)sys/resource.h $(INC)sys/time.h 
param.o: $(INC)sys/sysctl.h $(INC)sys/kernel.h 
sysconf.o: $(INC)sys/sysctl.h $(INC)sys/kernel.h
mm.o: $(INC)sys/mman.h $(INC)sys/mmanx.h
signal.o: $(INC)sys/signal.h $(INC)signal.h
