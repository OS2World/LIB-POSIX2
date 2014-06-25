/* 	dllPriv.c - (libExt+gcc) 	*/
/* $Header: /cvsroot/posix2/posix2/libext/dllPriv.c,v 1.2 2000/09/11 00:18:24 arnd Exp $ */

#include "rev.h"

const char 
_libExtensionsPrivateDLL_Revision[] = 	{ "\
	$Id: dllPriv.c,v 1.2 2000/09/11 00:18:24 arnd Exp $ -- libExt+gcc (_LIBEXT_REVISION_ID_HEADER__): " _LIBEXTNS_VPRT
};


/* This is the default _DLL_InitTerm() function for dynamic link libraries.
 */
	extern unsigned long
_DLL_InitTerm(unsigned long mod_handle, unsigned long flag)
{
    switch (flag) { /* initialize DLL */

      case 0: {
          if (_CRT_init() != 0)  /* init C runtime environment */
              return 0;		/* error! */
          __ctordtorInit(); 	/* construct dll statical objects */
        return(1);
      }
      case 1: {
          __ctordtorTerm ();  /* destruct statical objects */
#if 0
        _CRT_term(); 	/* for full-blown C runtime DLL: term C env */ 
#endif
        return(1);       /* sucess */
      }

      default:
        return(0);
    }
    return(1);
}
