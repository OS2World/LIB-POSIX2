/* $Id: debugx.h,v 1.5 2000/10/11 14:48:54 arnd Exp $ */

/* debugx.h - definitions for debugging libextensions - emx/gcc */

#ifndef __SYS_DEBUGX_H_
#define __SYS_DEBUGX_H_

#if defined(__cplusplus)
# ifndef __inline__ 
#  define __inline__ inline
# endif
extern "C" {
#endif

#ifdef DEBUG 		/* #undef DEBUG if you don't want debugging */
# include <stdio.h>
# define _dPuts(x) 		fputs((x),stderr)
# define _dPrintf(x,y) 		fprintf(stderr,(x),(y))
# define _dPrintf3(x,y,z) 	fprintf(stderr,(x),(y),(z))
# define _dPrintf4(w,x,y,z) 	fprintf(stderr,(w),(x),(y),(z))
# define _dPrintf5(v,w,x,y,z) 	fprintf(stderr,(v),(w),(x),(y),(z))
#else
# define _dPuts(x) 		/* empty */
# define _dPrintf(x,y) 		/* empty */
# define _dPrintf3(x,y,z) 	/* empty */
# define _dPrintf4(w,x,y,z) 	/* empty */
# define _dPrintf5(v,w,x,y,z) 	/* empty */
#endif

extern unsigned long int __popen_LLlength(void);
extern unsigned long int __popen_LLprint(void);

#if defined(__cplusplus)
}
#endif

#endif /* __SYS_DEBUGX_H_ */


