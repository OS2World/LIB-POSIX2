 /* sync.c -- emx/gcc */
#define __ST_MT_ERRNO__ 1

#define INCL_BASE
#define OS2_API32
#define INCL_DOSFILEMGR   /* File Manager values */
#include <os2.h>

#include <unistd.h>
#include <errno.h>
#include <sys/unistdx.h>
#include <sys/errnox.h>

  int
_sync(void)
{
 APIRET  rc = DosShutdown(0UL);

	if (rc) {
		errno = _rc2Errno(rc);
		return(-1);
	}
	return (0);
}
	int
sync(void) {return(_sync());}

