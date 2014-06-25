#	$Header: /cvsroot/posix2/posix2/libext/libc/db/Makefile,v 1.2 2000/11/25 16:33:35 arnd Exp $
#
# CFLAGS+=-D__DBINTERFACE_PRIVATE
#.include "${LIBCSRCDIR}/db/btree/Makefile.inc"
#.include "${LIBCSRCDIR}/db/db/Makefile.inc"
#.include "${LIBCSRCDIR}/db/hash/Makefile.inc"
#.include "${LIBCSRCDIR}/db/man/Makefile.inc"
#.include "${LIBCSRCDIR}/db/mpool/Makefile.inc"
#.include "${LIBCSRCDIR}/db/recno/Makefile.inc"

INC = ../../include/
MKINC 	= ../../../usr/share/mk/

AR     = ar rc
EMXOMF = emxomf
RANLIB = ar s
RM     = rm -f

all:: sub db.lib

sub:
	rm -f db.a
	cd btree && $(MAKE) all
	$(MAKE) db.o
	cd hash && $(MAKE) all
	cd mpool && $(MAKE) all
	cd recno && $(MAKE) all

db.a: 
	ar rcs db.a btree/*.o *.o hash/*.o mpool/*.o recno/*.o 

db.lib: db.a
	emxomf -l -s db.a

clean::
	$(RM) *.a *.lib *.o *.obj core a.out
	cd btree && $(MAKE) clean
	cd hash && $(MAKE) clean
	cd mpool && $(MAKE) clean
	cd recno && $(MAKE) clean

include ../../Ext.mk

DEFADD+= -D__DBINTERFACE_PRIVATE 


# dependencies
db.o: db.c $(INC)db.h
