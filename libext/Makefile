# $Header: /cvsroot/posix2/posix2/libext/Makefile,v 1.27 2000/11/27 22:17:16 arnd Exp $
# requires GNUmake 

INC      = ./include/
MKINC	   = ../usr/share/mk/
SUBCLEAN = subclean

LIB=

default: sub libs

sub:
	cd uni  && $(MAKE) all
	cd libc && $(MAKE) all && cp *.a *.lib ..
	cd libl && $(MAKE) all && cp *.a *.lib ..
	cd libz && $(MAKE) all && cp *.a *.lib ..
	cd libcom_err && $(MAKE) all && cp *.a *.lib ..
	cd libstcExt && $(MAKE) all && cp *.a *.lib ..
	cd libresolv && $(MAKE) all && cp *.a *.lib ..
	cd libutil && $(MAKE) all && cp *.a *.lib ..
	cd libcompat && $(MAKE) all && cp *.a *.lib ..
	cd librpcsvc && $(MAKE) all && cp *.a *.lib ..

test: cExt.a
	cd test && $(MAKE) test

run: test
	cd test && $(MAKE) run


## DLL-related targets

DLL= 		ExtGlob.DLL ExtPriv.DLL

DLLFLAGS=	-s -Zomf -Zmt -Zdll -Zcrtdll -Zlinker /NOI -Zlinker /EXEPACK:2 \
#		-g -Zlinker /DEBUG -Zlinker /M 

OMFLIBS= 	cExt.lib util.lib z.lib com_err.lib rpcsvc.lib 
PRIVOMFLIBS=	stcExt.lib
GLOB= 		extGlob.def dllGlob.c com_err.lib resolv.lib cExt.lib \
		extGlob.lib
PRIV=		dllPriv.c extGlob.lib extPriv.def extPriv.lib
LIBRARY=	-L. -lrpcsvc -lcom_err -lz -lutil -lcExt -lresolv -lsocket # -lstdcpp
PRIVLIBRARY=	-L. -lstcExt -lextGlob -lresolv -lsocket # -lstdcpp

dll: sub 
	$(RM) $(DLL) *.def
	$(MAKE) $(DLL)

extGlob.def: extGlob.def.in $(OMFLIBS) 
	$(CP) extGlob.def.in $@
	$(EMXEXP)20 $(OMFLIBS) >>$@

extPriv.def: extPriv.def.in $(PRIVOMFLIBS) 
	$(CP) extPriv.def.in $@
	$(EMXEXP)10 $(PRIVOMFLIBS) >>$@

extGlob.lib: extGlob.def
	$(EMXIMP) -o $@ $<
	$(EMXIMP) -o extGlob.a $<

extPriv.lib: extPriv.def
	$(EMXIMP) -o $@ $< 
	$(EMXIMP) -o extPriv.a $<

ext.lib: extGlob.lib extPriv.lib
	$(EMXOMFAR) $@ extGlob.lib extPriv.lib
	$(AR) ext.a extGlob.a extPriv.a

ExtGlob.DLL: $(GLOB)
	$(CC) -o $@ $(DLLFLAGS) extGlob.def dllGlob.c $(LIBRARY)
	$(BLDLEVEL) $@ 

ExtPriv.DLL: $(PRIV)
	$(CC) -o $@ $(DLLFLAGS) extPriv.def dllPriv.c $(PRIVLIBRARY)
	$(BLDLEVEL) $@ 

lite: 
	$(LITE) *.dll

## Dependencies
$(DLL): rev.h


libs: cExt.a uni/libuni.a uni/uconv.a uni/unikbd.a
	$(EMXOMFAR) cExt.lib uni/libuni.lib uni/uconv.lib uni/unikbd.lib
	$(AR) cExt.a uni/libuni.a uni/uconv.a uni/unikbd.a 


include Ext.mk   # specify tools and generic flags in 'Ext.mk'

## generic targets

clean:: veryclean

veryclean:: subclean
	$(RM) *.o *.obj *.lib *.a *.def *.DLL core *.tmp *.log

subclean: 
	cd test && $(MAKE) veryclean
	cd libc && $(MAKE) veryclean
	cd libz && $(MAKE) veryclean
	cd libl && $(MAKE) veryclean
	cd libstcExt && $(MAKE) veryclean
	cd libcom_err && $(MAKE) clean
	cd libresolv && $(MAKE) clean
	cd libutil && $(MAKE) clean
	cd libcompat && $(MAKE) clean
	cd librpcsvc && $(MAKE) clean
	cd uni && $(RM) *.lib *.a && cd ..
