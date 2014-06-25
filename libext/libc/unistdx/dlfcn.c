/* dlfcn.c -- emx/gcc -- Implementation of dlopen() interface for OS/2
   This code is released into public domain 
     */
/* $Id: dlfcn.c,v 1.2 2000/08/14 16:13:42 amai Exp $    */ 

/* Todo: Table of open HModule handles */

#define INCL_DOSPROCESS	/* Process api values */
#define INCL_DOSMODULEMGR	/* Module Manager values */
#define INCL_DOSERRORS		/* Error values */
#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

#define LM_LENGTH  256	/* length of error message string */

CHAR 
__dlfcn_loadError[LM_LENGTH] = ""; 	/* global msg string, 
	* containing the last error, e.g. returned by dlerror()
	*/
static CHAR 
__dlfcn_sysLoadError[LM_LENGTH]; 	/* argument to
 	* DosLoadModule, currently ignored, but maybe used upon debugging
	*/

	extern void *
_dlopen(const char *path, int dummy)
{
 HMODULE DLLHandle;
 APIRET rc;
 CHAR *tmpBuf = NULL;
 
	if (!path) { 			/* standard requires to return handle of main() ! */
	 PTIB dltib_p; PPIB dlpib_p; 		/* repository for info block pointers */

		/* get pointer to pib */
		if (!(rc = DosGetInfoBlocks(&dltib_p, &dlpib_p)) )
			fprintf(stderr, 
"dlfcn: dlopen(%s, %d): Warning: Returned handle is of main module!\n\
It cannot be used to resolve symbols dynamically. Fix me!\n", path, dummy);
			/* return value of module handle of current processes executable */
			return (void*)dlpib_p->pib_hmte;
	} else {
		if (!(tmpBuf = _strdup(path))) { 	/* strdup(path) for paranoia */
			sprintf(__dlfcn_loadError, "dlopen(%s): Out of memory error!", path);
			return(NULL);
		}
		rc = DosLoadModule((UCHAR*)__dlfcn_sysLoadError,LM_LENGTH - 1,
			(const UCHAR*)tmpBuf, &DLLHandle);
	}
	if (rc) {
		sprintf(__dlfcn_loadError, "DosLoadModule(\"%s\") = %lu\n", path, rc);
		if (tmpBuf) free(tmpBuf);
		return(NULL);
	}
	free(tmpBuf);
	return (void*)DLLHandle;
}

char
__dlfcn_Buf[LM_LENGTH] = ""; 	/* global temporary error message buffer */

	extern const char *
_dlerror(void)
{

  if (!strcmp(__dlfcn_loadError,""))
      return (char*)NULL;

	strncpy(__dlfcn_Buf, __dlfcn_loadError, LM_LENGTH - 1);
	strcpy(__dlfcn_loadError, ""); /* reset the error message */
	return(__dlfcn_Buf);
}

	extern void *
_dlsym(void *handle, __const__ char *symbol)
{
 PFN FuncAddress;  /* repository for address of the resolved symbol */
 __const__ APIRET rc = 
  DosQueryProcAddr((HMODULE)handle, 0L, (const UCHAR*)symbol, &FuncAddress);

  if (!rc) 			/* no error */
    return (void*)FuncAddress;  /* hooray, return entry point */

  switch (rc) {  		/* now we are in trouble */
    case ERROR_INVALID_HANDLE: {
	    sprintf (__dlfcn_loadError,
"DosQueryProcAddr(\"%s\")=ERROR_INVALID_HANDLE #: %lu\n", symbol, rc);
	    break;
    }
    case ERROR_ENTRY_IS_CALLGATE: {
	    sprintf (__dlfcn_loadError,
"DosQueryProcAddr(\"%s\")=ERROR_ENTRY_IS_CALLGATE?! #: %lu\n", symbol, rc);
	    break;
    }
    default: {
	    sprintf (__dlfcn_loadError, 
"DosQueryProcAddr(\"%s\") returned error code #: %lu\n", symbol, rc);
	    break;
    }
  }	/* end switch(rc) */
  return(NULL);
}

	extern int 
_dlclose (void *handle)
{
 __const__ APIRET rc = 
  DosFreeModule((HMODULE)handle);

  if (rc != NO_ERROR) {
    sprintf (__dlfcn_loadError, "DosFreeModule()=%lu\n", rc);
    return 2;
  }
	strcpy(__dlfcn_loadError, "");  /* reset last error (if any) */
  return 0;
}
	extern void *
dlopen(const char *p,int d){return _dlopen(p,d);}
	extern int 
dlclose(void *handle){return _dlclose(handle);}
	extern void *
dlsym(void*h,const char*s){return _dlsym(h,s);}
	extern const char *
dlerror(void){return _dlerror();}

