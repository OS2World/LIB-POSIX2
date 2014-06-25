/* daemon.c (libextensions/gcc) - $Id: daemon.c,v 1.7 2000/05/02 07:44:52 veit Exp $ */

#include <unistd.h>
#include <sys/types.h>		/* `off_t' etc. in struct flock in sys/fcntl.h */
#include <sys/stat.h>		/* POSIX orthodoxy (arnd) */
#include <fcntl.h>		/* modified to work with EMX_PTHREAD_REV 6     */
#include <stdlib.h>
#include <paths.h>

/* we need a real dev/null and a BSD paths.h mechanism */
#ifndef _PATH_DEVNULL
#define _PATH_DEVNULL "NUL"
#endif

/* OS/2 uses a detached spawned process, instead. Fix me!
 */
	extern int
_daemon(int nochdir, int noclose)
{
	switch (_fork()) {    /* rewrite with spawn()! */
	case -1:			/* fork() returned error and set errno */
		return(-1);
	case 0:				/* Scotty beamed us down to fork'd planet */
		break;
	default:
		_exit(0); 	/* Scotty shut down; no more beamup */
	}

	/* no sessiongroups, controling terminals, etc. yet */
	if (_setsid() == -1) 		/* just a dummy, for now */
		return(-1);

	if (!nochdir)
		(void)_chdir(_POSIX_OS2_ROOT);
		
	{ int fd;
	 if ( !noclose && (-1 != (fd = _open(_PATH_DEVNULL, O_RDWR|O_BINARY, 0))) ) {
		(void)_dup2(fd, STDIN_FILENO); /* 'close' std streams, use /dev/null */
		(void)_dup2(fd, STDOUT_FILENO);
		(void)_dup2(fd, STDERR_FILENO);

		if (fd > STDERR_FILENO)
			(void)_close(fd);
	 } 
	}
	return(0);
}

	extern int
daemon(int nochdir, int noclose) {return _daemon(nochdir,noclose);}
