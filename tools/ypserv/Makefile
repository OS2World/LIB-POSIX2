#	$OpenBSD: Makefile,v 1.6 1997/07/26 12:12:26 maja Exp $

SUBDIR=	makedbm mkalias mknetid revnetgroup stdethers stdhosts \
	ypinit ypserv yptest yppush ypxfr

all:: sub

sub:
	cd makedbm && $(MAKE) all
	cd mkalias && $(MAKE) all
	cd mknetid && $(MAKE) all
	cd revnetgroup && $(MAKE) all
	cd stdethers && $(MAKE) all
	cd stdhosts && $(MAKE) all
	cd ypinit && $(MAKE) all
	cd ypserv && $(MAKE) all
	cd yptest && $(MAKE) all
	cd yppush && $(MAKE) all
	cd ypxfr&& $(MAKE) all


clean:: subclean
	$(RM) *.exe core *.o *.a *.lib

subclean:
	cd makedbm && $(MAKE) clean
	cd mkalias && $(MAKE) clean
	cd mknetid && $(MAKE) clean
	cd revnetgroup && $(MAKE) clean
	cd stdethers && $(MAKE) clean
	cd stdhosts && $(MAKE) clean
	cd ypinit && $(MAKE) clean
	cd ypserv && $(MAKE) clean
	cd yptest && $(MAKE) clean
	cd yppush && $(MAKE) clean
	cd ypxfr&& $(MAKE) clean

include tools.mk
