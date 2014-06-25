#	$Id: Makefile,v 1.3 2000/08/18 18:44:14 amai Exp $
#
# By default, flex will be configured to generate 8-bit scanners only if the
# -8 flag is given.  If you want it to always generate 8-bit scanners, add
# "-DDEFAULT_CSIZE=256" to CFLAGS.  Note that doing so will double the size
# of all uncompressed scanners.
# 
# If on your system you have trouble building flex due to 8-bit character
# problems, remove the -8 from FLEX_FLAGS and the "#define FLEX_8_BIT_CHARS"
# from the beginning of flexdef.h.
#
# To bootstrap lex, cp initscan.c to scan.c and run make.

PROG=	lex
SRCS=	ccl.c dfa.c ecs.c gen.c main.c misc.c nfa.c parse.c sym.c tblcmp.c \
	yylex.c skel.c scan.c 

CLEANFILES+=parse.c parse.h scan.c skel.c y.tab.c y.tab.h bootstrap
LDADD=	-ll

MAN = flex.1

#LINKS=	${BINDIR}/lex ${BINDIR}/flex \
	${BINDIR}/lex ${BINDIR}/flex++
MLINKS=	flex.1 lex.1 


include ../tools.mk


parse.h parse.c: parse.y
	${YACC} -d $<
	mv y.tab.c parse.c
	mv y.tab.h parse.h

skel.c:	flex.skl mkskel.sh
	sh ./mkskel.sh ./flex.skl > ./skel.c

scan.c: scan.l bootstrap
	$(LEX) -t -p ./scan.l > ./scan.c


bootstrap:
	cp -f initscan.c scan.c
	touch scan.c bootstrap
	$(MAKE) $(PROG).exe && $(CP) $(PROG).exe ..


scan.o: parse.c
