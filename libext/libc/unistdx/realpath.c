/*
 * Copyright (c) 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Jan-Simon Pendry.
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


/* no symbolic links to read are implemented for now */
#define	S_ISLNK(m) (0) 				/* FIXME! :-( */

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

/*
 * char *realpath(const char *path, char resolved_path[MAXPATHLEN]);
 *
 * Find the real name of path, by removing all ".", ".." and symlink
 * components.  Returns (resolved) on success, or (NULL) on failure,
 * in which case the path which caused trouble is left in (resolved).
 */
	extern char *
realpath(const char *path, char resolved[MAXPATHLEN])
{
	register char *c_p;
	struct stat sb;
	int n, rootd, serrno;
	char *p, wbuf[MAXPATHLEN];
	int symlinks = 0;
	/* Save the starting point. */
	const char *cwd_p = _getcwd(resolved, MAXPATHLEN);

	if (!cwd_p) 
		return (NULL);

	/*
	 * Find the dirname and basename from the path to be resolved.
	 * Change directory to the dirname component.
	 * lstat the basename part.
	 *     if it is a symlink, read in the value and loop.
	 *     if it is a directory, then change to that directory.
	 * get the current directory name and append the basename.
	 */
	(void)strncpy(resolved, path, MAXPATHLEN - 1);
	resolved[MAXPATHLEN - 1] = '\0';
loop:
	c_p = strrchr(resolved, '/');
	if (c_p == NULL)
		c_p = strrchr(resolved, '\\');
	if (c_p != NULL) { 	/* strip anything after trailing (back-)slash */
		p = c_p + 1; /* now pointing to last component */
		if (c_p == resolved)
			c_p = "/";
		else {
			do {
				--c_p;
			} while (c_p > resolved && (*c_p == '/' || *c_p == '\\'));
			c_p[1] = '\0';
			c_p = resolved;
		}
		if (chdir(c_p) < 0)
			goto err1;
	} else 		/* no trailing slash */
		p = resolved;

	/* Deal with the last component. */
	if (lstat(p, &sb) == 0) {
		if (S_ISLNK(sb.st_mode)) {
			if (++symlinks > MAXSYMLINKS) {
				errno = ELOOP;
				goto err1;
			}
			n = readlink(p, resolved, MAXPATHLEN-1);
			if (n < 0)
				goto err1;
			resolved[n] = '\0';
			goto loop;
		}
		if (S_ISDIR(sb.st_mode)) {
			if (chdir(p) < 0)
				goto err1;
			p = "";
		}
	}

	/*
	 * Save the last component name and get the full pathname of
	 * the current directory (drive letter excluded).
	 */
	(void)strcpy(wbuf, p);
	if (getcwd(resolved, MAXPATHLEN) == 0)
		goto err1;
	/*
	 * Join the two strings together, ensuring that the right thing
	 * happens if the last component is empty, or the dirname is root.
	 */
	if ( (resolved[0] == '/' || resolved[0] == '\\') && resolved[1] == '\0' )
		rootd = 1;
	else
		rootd = 0;

	if (*wbuf) {
		if (strlen(resolved) + strlen(wbuf) + rootd + 1 > MAXPATHLEN) {
			errno = ENAMETOOLONG;
			goto err1;
		}
		if (rootd == 0)
			(void)strcat(resolved, "/");
		(void)strcat(resolved, wbuf);
	}
	/* Go back to where we came from; test path, make slashes. */
	if ( chdir(cwd_p) < 0 
	  || _fullpath(resolved, resolved, MAXPATHLEN) ) {
		serrno = errno;
		goto err2;
	}
	return (resolved);

err1:	serrno = errno;
	(void)chdir(cwd_p);
err2:	errno = serrno;
	return(NULL);
}

