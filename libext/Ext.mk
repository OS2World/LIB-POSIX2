# $Header: /cvsroot/posix2/posix2/libext/Ext.mk,v 1.5 2000/11/25 16:33:35 arnd Exp $

#For easy debugging do not use gcc '__builtin_' and inline functions:
# add: '-fno-builtin -fno-inline'

#Add additional variables for all libraries here:

# flags section

DEBUG	= -O2 -g -mpentium -D_DEBUG -DDEBUG # -fno-builtin -fno-inline 
DEFADD= -D_USE_HRTIMER_SYS -D_CPYFILE_FOR_SYMLINK -DNLS -DYP 

# tools section
CC = gcc
CP = cp -f
LN = cp -f
RM = rm -f
AR = ar rcs
RANLIB = ar s
IPFC   = ipfc
LATEX  = latex
LD     = gcc
LEX    = flex
SED    = sed
TEX    = latex
YACC   = bison
AWK    = gawk

EMXOMFAR = emxomfar rcs
EMXOMF = emxomf -l -s
EMXIMP = emximp
EMXEXP = emxexp -n -o
RC     = rc

CTAGS  =	ctags -R --c-types=+p+x 
DVIPS  = 	dvips
LITE   = 	lxlite
BLDLEVEL= 	bldlevel


##
include $(MKINC)Posix2Lib.mk  # the generic Makefile parts
##

#Add override flags for all libraries here:


##
