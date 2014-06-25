# $Header: /cvsroot/posix2/posix2/tools/tools.mk,v 1.7 2000/11/17 19:17:33 arnd Exp $

DESTDIR = ../../libext
MKINC =	../../usr/share/mk/Posix2App.mk

LIBEXT = ../../libext/cExt.a ../../libext/stcExt.a 

#For easy debugging do not use gcc '__builtin_' and inline functions:
# add: '-fno-builtin -fno-inline'
#DEBUG   = -g -O2 -mpentium -D_DEBUG -DDEBUG -D_THREAD_EXPERIMENTAL \
	 -fno-builtin -fno-inline 
##

#Add additional variables for all tools here:

# tools section

CC = gcc
CP = cp -f
AR = ar rcs
RM = rm -f

EMXOMF = emxomf -l -s
EMXEXP = emxexp
RANLIB = ar s
EMXOMFAR = emxomfar rcs

CTAGS  = ../ctags/ctags
YACC = ../yacc/yacc
LEX  = ../lex/lex
MK   = ../pmake -m ../../usr/share/mk

# flags section

INCLUDE = -I. -I../../libext/include 

DEBUG   = -g -O2 -mpentium -D_DEBUG -DDEBUG -DTHREAD_EXPERIMENTAL 

SYSLIB = $(LIBEXT)
LIBRARY= -L../../libext/ -lcExt -lstcExt -lsocket #-lstdcpp 
DEF    = ../EXE.DEF


##
include $(MKINC)
##

#Add override definitions for all libraries here:



##

