# $Header: /cvsroot/posix2/posix2/libext/libc/unistdx/Makefile,v 1.18 2000/11/25 16:33:36 arnd Exp $

INC = ../../include/
MKINC 	= ../../../usr/share/mk/

# Define the relevant options that your system configuration supports:
#
# POSIX2_CHAR_TERM
# Define if if the system supports at least one terminal type capa-
#             ble of all operations described in POSIX 1003.2.
# POSIX2_C_BIND
# Define if if the system's C-language development facilities sup-
#             port the C-Language Bindings Option.
# POSIX2_C_DEV
# Define if if the system supports the C-Language Development Utili-
#             ties Option.
# POSIX2_FORT_DEV
# Define if if the system supports the FORTRAN Development Utilities
#             Option.
# POSIX2_FORT_RUN
# Define if if the system supports the FORTRAN Runtime Utilities Op-
#             tion.
# POSIX2_LOCALEDEF
# Define if if the system supports the creation of locales, other-
#             wise 0.
# POSIX2_SW_DEV
# Define if if the system supports the Software Development Utili-
#             ties Option.
# POSIX2_UPE
# Define if if the system supports the User Portability Utilities
#             Option.

SRCS    = alarm.c dlfcn.c getwd.c lockf.c msgx.c nice.c \
 poll.c pwcache.c realpath.c shmx.c symlink.c \
 usleep.c fts.c syslog.c search.c direntx.c setmode.c glob.c \


HEADERS	= $(INC)sys/types.h $(INC)unistd.h $(INC)sys/unistd.h \
	  $(INC)sys/unistdx.h $(INC)sys/errnox.h 


include ../../Ext.mk

DEFADD+= -DPOSIX2_C_BIND -DPOSIX2_C_DEV -DPOSIX2_SW_DEV

## Dependencies
alarm.o: $(INC)sys/time.h $(INC)sys/itimer.h
lockf.o: $(INC)fcntl.h
pwcache.o: $(INC)pwd.h
grpx.o: $(INC)grp.h
poll.o: $(INC)sys/poll.h 
dlfnc.o: $(INC)dlfcn.h 
msgx.o: $(INC)sys/ipc.h $(INC)sys/msg.h
shmx.o: $(INC)sys/ipc.h $(INC)sys/shm.h
fts.o: $(INC)fts.h
search.o: $(INC)search.h
