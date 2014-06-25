# $Header: /cvsroot/posix2/posix2/libext/uni/Makefile,v 1.3 2000/08/23 21:47:06 amai Exp $
#  requires GNUmake

.SUFFIXES:

.SUFFIXES: .def .lib .a

EMXIMP = emximp
RM     = rm -f

default: libuni.a uconv.a unikbd.a libuni.lib uconv.lib unikbd.lib

all: default

.def.a:
	$(EMXIMP) -o $@ $*.def

.def.lib:
	$(EMXIMP) -o $@ $*.def
