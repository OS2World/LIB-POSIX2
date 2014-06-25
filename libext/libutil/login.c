/* $Id: login.c,v 1.1 2000/11/26 16:54:58 arnd Exp $	*/
/*
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#if defined(LIBC_SCCS) && !defined(lint)
/* from: static char sccsid[] = "@(#)login.c	8.1 (Berkeley) 6/4/93"; */
static char *rcsid = "$Id: login.c,v 1.1 2000/11/26 16:54:58 arnd Exp $";
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <utmp.h>

#include "util.h"

	extern void
login(struct utmp *utp)
{
	struct utmp old_ut;
	register int fd;
	int tty = ttyslot();

	if (tty > 0 && (fd = open(_PATH_UTMP, O_RDWR|O_CREAT, 0644)) >= 0) {
		(void)_lseek(fd, (off_t)(tty * sizeof(struct utmp)), SEEK_SET);
		/*
		 * Prevent luser from zero'ing out ut_host.
		 * If the new ut_line is empty but the old one is not
		 * and ut_line and ut_name match, preserve the old ut_line.
		 */
		if (_read(fd, &old_ut, sizeof(struct utmp)) ==
		    sizeof(struct utmp) && utp->ut_host[0] == '\0' &&
		    old_ut.ut_host[0] != '\0' &&
		    _strncmp(old_ut.ut_line, utp->ut_line, UT_LINESIZE) == 0 &&
		    _strncmp(old_ut.ut_name, utp->ut_name, UT_NAMESIZE) == 0)
			(void)memcpy(utp->ut_host, old_ut.ut_host, UT_HOSTSIZE);
		(void)_lseek(fd, (off_t)(tty * sizeof(struct utmp)), SEEK_SET);
		(void)_write(fd, utp, sizeof(struct utmp));
		(void)_close(fd);
	}
	if ((fd = _open(_PATH_WTMP, O_WRONLY|O_APPEND, 0)) >= 0) {
		(void)_write(fd, utp, sizeof(struct utmp));
		(void)_close(fd);
	}
}

	extern int
login_tty(int fd)
{
	(void) _setsid();
	if (_ioctl(fd, TIOCSCTTY, (char *)NULL) == -1)
		return (-1);
	(void) _dup2(fd, 0);
	(void) _dup2(fd, 1);
	(void) _dup2(fd, 2);
	if (fd > 2)
		(void) _close(fd);
	return (0);
}

typedef struct utmp UTMP;

	extern int
logout(const char *line)
{
	int fd, rval;
	UTMP ut;

	if ((fd = _open(_PATH_UTMP, O_RDWR, 0)) < 0)
		return(0);
	rval = 0;
	while (_read(fd, &ut, sizeof(UTMP)) == sizeof(UTMP)) {
		if (!ut.ut_name[0] || strncmp(ut.ut_line, line, UT_LINESIZE))
			continue;
		bzero(ut.ut_name, UT_NAMESIZE);
		bzero(ut.ut_host, UT_HOSTSIZE);
		(void)time(&ut.ut_time);
		(void)_lseek(fd, -(off_t)sizeof(UTMP), SEEK_CUR);
		(void)_write(fd, &ut, sizeof(UTMP));
		rval = 1;
	}
	(void)_close(fd);
	return(rval);
}
	extern void
logwtmp(const char *line, const char *name, const char *host)
{
	struct utmp ut;
	struct stat buf;
	int fd;

	if ((fd = _open(_PATH_WTMP, O_WRONLY|O_APPEND, 0)) < 0)
		return;
	if (_fstat(fd, &buf) == 0) {
		(void) strncpy(ut.ut_line, line, sizeof(ut.ut_line));
		(void) strncpy(ut.ut_name, name, sizeof(ut.ut_name));
		(void) strncpy(ut.ut_host, host, sizeof(ut.ut_host));
		(void) time(&ut.ut_time);
		if (_write(fd, (char *)&ut, sizeof(struct utmp)) !=
		    sizeof(struct utmp))
			(void) _ftruncate(fd, buf.st_size);
	}
	(void) _close(fd);
}
