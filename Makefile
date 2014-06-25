# Posix/2-Project Toplevel Maintainer Makefile
#$Header: /cvsroot/posix2/posix2/Makefile,v 1.9 2000/08/14 16:17:00 amai Exp $

#fix the tree (the \OS2\FIND.EXE will NOT work!!!):
#	\emx\bin\find . -type f -exec fixtxt {} ;

# Remove the garbage fixcrcr.exe.tmp file in the toplevel directory.

#WARNING: The find command will apply fixcrcr to ALL files in the tree, and
#will thus also destroy existing binaries. If you have updated the tree
#from the repository, you have to "make clean" before recompiling, otherwise
#you will produce defective files.

RM = rm -f

CFLAGS = -Wall -W -Zomf -Zsys -s -O2 -mpentium -Zlinker /EXEPACK:2

default: all

all: fixtxt.exe

fixtree: fixtxt.exe
	-find . -type f -not -name "*.exe" -not -name "*.tmp" -not -name "*.pdf" \
 -not -name "*.cmd" -not -name "*.lib" -not -name "*.a" -not -name "*.gif" \
 -exec fixtxt.exe {} ";" -print
	$(RM) *.tmp *.exe

fixtxt.exe: fixcrcr.c libext/test/EXE.DEF
	gcc $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o *.obj *.res *.aux *.log *.toc dummy *.exe core *.tmp

veryclean: clean
	cd libext && $(MAKE) veryclean
	cd tools && $(MAKE) veryclean

distclean: veryclean fixtree

maintainerclean: distclean
	
