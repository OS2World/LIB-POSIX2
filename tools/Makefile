# Posix/2-Project Maintainer Makefile
#$Header: /cvsroot/posix2/posix2/tools/Makefile,v 1.16 2000/11/25 16:33:36 arnd Exp $

LIBEXT = ../libext/cExt.a ../libext/stcExt.a 
CC = gcc
MV = mv -f

all: buildtools tools 

$(LIBEXT):
	cd .. && cd libext && $(MAKE)

tools: $(LIBEXT)
	cd apply && $(MAKE) all 
	cd apropos && $(MAKE) all 
	cd arch && $(MAKE) all 
	cd asa && $(MAKE) all 
	cd awk && $(MAKE) all 
#	cd calendar && $(MAKE) all 
	cd cat && $(MAKE) all 
	cd catman && $(MAKE) all 
	cd cmp && $(MAKE) all 
	cd col && $(MAKE) all 
	cd cut && $(MAKE) all 
	cd echo && $(MAKE) all 
	cd env && $(MAKE) all 
	cd error && $(MAKE) all 
	cd expand && $(MAKE) all 
	cd expr && $(MAKE) all 
#	cd find && $(MAKE) all 
	cd fmt && $(MAKE) all 
	cd fold && $(MAKE) all 
#	cd fsplit && $(MAKE) all 
	cd gencat && $(MAKE) all 
	cd getopt && $(MAKE) all 
	cd head && $(MAKE) all 
	cd hexdump && $(MAKE) all 
#	cd id && $(MAKE) all 
	cd join && $(MAKE) all 
	cd jot && $(MAKE) all 
	cd kill && $(MAKE) all 
	cd lam && $(MAKE) all 
	cd logger && $(MAKE) all 
	cd look && $(MAKE) all 
	cd lorder && cp *.sh ..
	cd ls && $(MAKE) all 
	cd m4 && $(MAKE) all 
#	cd man && $(MAKE) all 
	cd mesg && $(MAKE) all 
	cd mv && $(MAKE) all 
	cd nice && $(MAKE) all 
	cd paste && $(MAKE) all 
	cd pr && $(MAKE) all 
	cd printenv && $(MAKE) all 
#	cd ps && $(MAKE) all 
	cd pwd && $(MAKE) all 
	cd rev && $(MAKE) all 
	cd rm && $(MAKE) all 
	cd rmdir && $(MAKE) all 
	cd rs && $(MAKE) all 
#	cd script && $(MAKE) all 
	cd sed && $(MAKE) all 
	cd shar && cp *.sh ..
	cd soelim && $(MAKE) all 
#	cd sort && $(MAKE) all 
	cd split && $(MAKE) all 
	cd sync && $(MAKE) all 
	cd tail && $(MAKE) all 
#	cd talk && $(MAKE) all 
	cd tee && $(MAKE) all 
	cd test && $(MAKE) all 
	cd tr && $(MAKE) all 
	cd tsort && $(MAKE) all 
	cd uname && $(MAKE) all 
	cd unexpand && $(MAKE) all 
	cd uniq && $(MAKE) all 
	cd uudecode && $(MAKE) all 
	cd uuencode && $(MAKE) all 
	cd unvis && $(MAKE) all 
	cd vgrind && $(MAKE) all
	cd vis && $(MAKE) all 
	cd wc && $(MAKE) all 
	cd what && $(MAKE) all 
	cd whatis && $(MAKE) all 
	cd write && $(MAKE) all 
	cd xargs && $(MAKE) all 
	cd xinstall&& $(MAKE) all 
	cd yes && $(MAKE) all 
	cd ypbind && $(MAKE) all 
	cd yppoll && $(MAKE) all 
	cd ypset && $(MAKE) all 


buildtools: $(LIBEXT)
	cd pmake && $(MAKE) all
	cd yacc  && $(MAKE) all 
	cd lex   && $(MAKE) all 
	cd cap_mkdb && $(MAKE) all 
#	cd compile_et && $(MAKE) all 
	cd ctags && $(MAKE) all 
	cd indent && $(MAKE) all 
	cd patch && $(MAKE) all 
	cd mkstr && $(MAKE) all 
	cd xstr && $(MAKE) all 


subclean:
	-DEL *.exe *.o *.obj *.res *.aux *.log *.toc core *.a dummy exp
	cd apply && $(MAKE) clean
	cd apropos && $(MAKE) clean
	cd arch && $(MAKE) clean
	cd asa && $(MAKE) clean
	cd awk && $(MAKE) clean
	cd calendar && $(MAKE) clean
	cd cat && $(MAKE) clean
	cd catman && $(MAKE) clean
	cd cap_mkdb && $(MAKE) clean
	cd chmod && $(MAKE) clean
	cd cmp && $(MAKE) clean
	cd col && $(MAKE) clean
#	cd compile_et && $(MAKE) clean
	cd ctags && $(MAKE) clean
	cd cp && $(MAKE) clean
	cd cut && $(MAKE) clean
	cd echo && $(MAKE) clean
	cd ed && $(MAKE) clean
	cd env && $(MAKE) clean
	cd error && $(MAKE) clean
	cd expand && $(MAKE) clean
	cd expr && $(MAKE) clean
	cd file2c && $(MAKE) clean
	cd find && $(MAKE) clean
	cd fmt && $(MAKE) clean
	cd fold && $(MAKE) clean
	cd fsplit && $(MAKE) clean
	cd gencat && $(MAKE) clean
	cd getconf && $(MAKE) clean
	cd getopt && $(MAKE) clean
	cd head && $(MAKE) clean
	cd hexdump && $(MAKE) clean
	cd indent && $(MAKE) clean
	cd id && $(MAKE) clean
	cd join && $(MAKE) clean
	cd jot && $(MAKE) clean
	cd kill && $(MAKE) clean
	cd lam && $(MAKE) clean
	cd lex && $(MAKE) clean
	cd logger && $(MAKE) clean
	cd logname && $(MAKE) clean
	cd look && $(MAKE) clean
	cd lorder && $(MAKE) clean
	cd ls && $(MAKE) clean
	cd m4 && $(MAKE) clean
	cd man && $(MAKE) clean
	cd mesg && $(MAKE) clean
	cd mktemp && $(MAKE) clean
	cd mkstr && $(MAKE) clean
	cd mv && $(MAKE) clean
	cd nice && $(MAKE) clean
	cd paste && $(MAKE) clean
	cd patch && $(MAKE) clean
	cd pmake && $(MAKE) clean
	cd pr && $(MAKE) clean
	cd printenv && $(MAKE) clean
	cd ps && $(MAKE) clean
	cd pwd && $(MAKE) clean
	cd rev && $(MAKE) clean
	cd rm && $(MAKE) clean
	cd rmdir && $(MAKE) clean
	cd rpcgen && $(MAKE) clean
	cd rpcinfo && $(MAKE) clean
	cd portmap && $(MAKE) clean
	cd rpc.bootparamd && $(MAKE) clean
	cd rpc.lockd && $(MAKE) clean
	cd rpc.pcnfsd && $(MAKE) clean
	cd rs && $(MAKE) clean
	cd script && $(MAKE) clean
	cd sed && $(MAKE) clean
	cd shar && $(MAKE) clean
	cd soelim && $(MAKE) clean
	cd sort && $(MAKE) clean
	cd split && $(MAKE) clean
	cd sync && $(MAKE) clean
	cd tail && $(MAKE) clean
	cd talk && $(MAKE) clean
	cd tee && $(MAKE) clean
	cd test && $(MAKE) clean
	cd touch && $(MAKE) clean
	cd tr && $(MAKE) clean
	cd tsort && $(MAKE) clean
	cd uname && $(MAKE) clean
	cd unexpand && $(MAKE) clean
	cd uniq && $(MAKE) clean
	cd unvis && $(MAKE) clean
	cd uudecode && $(MAKE) clean
	cd uuencode && $(MAKE) clean
	cd vgrind && $(MAKE) clean
	cd vis && $(MAKE) clean
	cd wc && $(MAKE) clean
	cd what && $(MAKE) clean
	cd whatis && $(MAKE) clean
	cd write && $(MAKE) clean
	cd xargs && $(MAKE) clean
	cd xinstall && $(MAKE) clean
	cd xlint && $(MAKE) clean
	cd xstr && $(MAKE) clean
	cd yacc && $(MAKE) clean
	cd yes && $(MAKE) clean
	cd ypbind && $(MAKE) clean
	cd yppoll && $(MAKE) clean
	cd ypset && $(MAKE) clean

clean: subclean

veryclean: subclean

maintainerclean: veryclean
