/* rev.h - libExt internal revision header - emx+gcc */
/* $Header: /cvsroot/posix2/posix2/libext/rev.h,v 1.20 2000/11/26 16:43:01 arnd Exp $ */

#ifndef _LIBEXT_REV_H_
#define _LIBEXT_REV_H_

/* increments with every new internal revision */
#define _LIBEXTNS_REV  0x00F		/* Format: 0x000  */
#define _LIBEXTNS_VPRT "0.0.15b" 	/* Format: "0.0.0" */

#define _LIBEXT_REVISION_ID_HEADER__	$Header: /cvsroot/posix2/posix2/libext/rev.h,v 1.20 2000/11/26 16:43:01 arnd Exp $

extern __const__ char __Rev_ostype[]; 
extern __const__ char __Rev_osrelease[]; 
extern __const__ char __Rev_osversion[]; 
extern __const__ char __Rev_version[]; 
extern __const__ char sccs[8];

extern unsigned long _DLL_InitTerm(unsigned long,unsigned long);
extern int _CRT_init(void);
extern void _CRT_term(void);
extern void __ctordtorInit(void);
extern void __ctordtorTerm(void);

#endif /* _LIBEXT_REV_H_ */
