/*	$OpenBSD: basename.c,v 1.4 1999/05/30 17:10:30 espie Exp $	*/

/*
 * Copyright (c) 1997 Todd C. Miller <Todd.Miller@courtesan.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <errno.h>
#include <libgen.h>
#include <string.h>
#include <sys/param.h>

	extern char *
basename(const char *path)
{
 static char bname[MAXPATHLEN] = "/";
 register const char *endp, *startp;
 size_t len;

	/* Empty or NULL string gets treated as "." */
	if (path == NULL || *path == '\0') {
		(void)strcpy(bname, ".");
		return(bname);
	}
	endp = path + strlen(path) - 1;

	while ( endp > path && (*endp == '/' || *endp == '\\') )
		--endp;		/* Strip trailing slashes */

	if ( endp == path && (*endp == '/' || *endp == '\\') )
		return(bname);

	/* Find the start of the base */
	startp = endp;
	while (startp > path && *(startp - 1) != '/' 
		   && *(startp - 1) != '\\')
		--startp; 	/* next (back-)slash */

	len = endp - startp + 1;
	if ( len > sizeof(bname)) {
		errno = ENAMETOOLONG;
		return(NULL);
	}
	(void)strncpy(bname, startp, len);
	bname[len] = '\0';
	return(bname);
}

	extern char *
dirname(const char *path)
{
 static char bname[MAXPATHLEN] = ".";
 register const char *endp;
 size_t len;

	/* Empty or NULL string gets treated as "." */
	if (path == NULL || *path == '\0') 
		return(bname);

	endp = path + strlen(path) - 1;
	while ( endp > path && (*endp == '/' || *endp == '\\') )
		--endp;		/* Strip trailing slashes */

	/* Find the start of the dir */
	while ( endp > path && (*endp == '/' || *endp == '\\') )
		--endp;

	if (endp == path) { /* Either the dir is "/" or there are no slashes */
		if (*endp == '/' || *endp == '\\') 
			(void)strcpy(bname, "/");
		return(bname);
	} else do 
		--endp;
	while ( endp > path && (*endp == '/' || *endp == '\\') );
	
	len = endp - path + 1; 
	if (len > sizeof(bname)) {
		errno = ENAMETOOLONG;
		return(NULL);
	}
	(void)strncpy(bname, path, len);
	bname[len] = '\0';
	return(bname);
}

