/* 	dll.c - (libExt+gcc) 	*/
/* $Header: /cvsroot/posix2/posix2/libext/dllGlob.c,v 1.1 2000/08/18 19:17:47 amai Exp $ */


#define INCL_DOSFILEMGR
#include <os2.h>

#include <sys/param.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/unistdx.h>
#include <float.h>
#include <errno.h>
#include <sys/errnox.h>
#include "rev.h"

const char 
_libExtensionsGlobalDLL_Revision[] = 	{ "\
	$Id: dllGlob.c,v 1.1 2000/08/18 19:17:47 amai Exp $ -- libExt+gcc (_LIBEXT_REVISION_ID_HEADER__): " _LIBEXTNS_VPRT
};


/* This is the default _DLL_InitTerm() function for dynamic link libraries.
 */
	extern unsigned long
_DLL_InitTerm(unsigned long mod_handle, unsigned long flag)
{
    switch (flag) { /* initialize DLL */

      case 0: {
        const APIRET 
          rc = DosSetMaxFH(OPEN_MAX);
          if (rc) {errno = _rc2Errno(rc); return(0);} /* error! */
          if (_CRT_init() != 0)  /* init C runtime environment */
              return 0;		/* error! */
          _control87(PC_53, MCW_PC); /* is this necessary ? */
          __ctordtorInit(); 	/* construct dll statical objects */
          __SHMx_shmInit(); 	/* cf. shmx.c */
          _initHrTimer(); 	/* cf. usleep.c */
        return(1);
      }
      case 1: {
          __SHMx_shmDone();
          _termHrTimer();
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
