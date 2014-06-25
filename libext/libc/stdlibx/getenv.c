/* $Id: getenv.c,v 1.2 2000/08/06 20:30:27 amai Exp $
 * Copyright (c) 1987, 1993
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

#include <stdlib.h>
#include <string.h>
#include <emx/startup.h>

/*
 * __getenv_findEnv --
 *	Returns pointer to value associated with name, if any, else NULL.
 *	Sets offset to be the offset of the name/value combination in the
 *	environmental array, for use by setenv(3) and unsetenv(3).
 *	Explicitly removes '=' in argument name.
 *
 *	This routine *should* be a static; don't use it.
 */
	extern char *
__getenv_findEnv(register const char *name, size_t *offset)
{
	register size_t namelen, i;
	register const char *np = name;
	register char **p = _environ, *cp;

	if (name == NULL || _environ == NULL)
		return (NULL);
	while (*np && *np != '=') /* advance to end of name */
		 ++np;
	namelen = np - name;

	while ((cp = *p) != NULL) { /* loop though _environ strings */
		for (np = name, i = namelen; i && *cp; --i)
			if (*cp++ != *np++)
				break; 		/* mismatch */
		if (i == 0 && *cp++ == '=') {
			if (offset) 
				*offset = p - _environ;
			return (cp);
		}
		++p;
	}
	return (NULL);
}

/*
 * getenv --
 *	Returns ptr to value associated with name, if any, else NULL.
 */
	extern char *
_getenv(const char *name)
{
	return __getenv_findEnv(name, NULL);
}

	extern char *
getenv(const char *name)
{
	return __getenv_findEnv(name, NULL);
}

/*
 * setenv --
 *	Set the value of the _environmental variable "name" to be
 *	"value".  If rewrite is set, replace any current value.
 */
	extern int
_setenv(register const char *name, register const char *value, int rewrite)
{
	register char *C;
	size_t l_value, offset;

	if (*value == '=')			/* no `=' in value */
		++value;
	l_value = strlen(value);
	if ((C = __getenv_findEnv(name, &offset))) {	/* find if already exists */
		if (!rewrite)
			return (0);
		if (strlen(C) >= l_value) {	/* old larger; copy over */
			while ( (*C++ = *value++) )
			;
			return (0);
		}
	} else {					/* create new slot */
	 register int	cnt = 0;
	 register char	**P = _environ;
		while (*P)
			{++P; ++cnt;}
		/* increase size or malloc() new, if _environ == app's original one */
		if (_environ == _org_environ) {  /* This not yet returned by malloc: */ 
			P = malloc( (cnt + 2) * sizeof(char*) );  /* cannot realloc()! */
			if (P == NULL)
				return -1;
			_environ = P;
			if (cnt)
				memcpy( _environ, _org_environ, cnt * sizeof(char*) );
		} else {
			P = (char**)realloc( _environ, sizeof(char*) * (cnt + 2) );
			if (!P)
				return(-1);
			_environ = P;
		}
		_environ[cnt + 1] = NULL;
		offset = cnt;
	}
	for (C = (char*)name; *C && *C != '='; ++C);	/* no `=' in name */
	if (!(_environ[offset] =			/* name + `=' + value */
	    malloc((size_t)((int)(C - name) + l_value + 2))))
		return (-1);
		
	C = _environ[offset]; 
	while ((*C = *name++) && *C != '=')
		++C;

	*C++ = '='; 
	while ((*C++ = *value++))
		;

	return (0);
}

/*
 * unsetenv(name) --
 *	Delete _environmental variable "name".
 */
	extern void
unsetenv(const char *name)
{
	register char **P;
	size_t offset;

	while (__getenv_findEnv(name, &offset))		/* if set multiple times */
		for (P = &_environ[offset];; ++P)
			if (!(*P = *(P + 1)))
				break;
}

	extern int
_putenv(const char *str)
{
	char *p, *equal;
	int rval;

	if ((p = _strdup(str)) == NULL)
		return (-1);
	if ((equal = strchr(p, '=')) == NULL) {
		(void)free(p);
		return (-1);
	}
	*equal = '\0';
	rval = _setenv(p, equal + 1, 1);
	(void)free(p);
	return (rval);
}

	extern int
setenv(register const char *name, register const char *value, int rewrite)
{return _setenv(name,value,rewrite);}
	extern int
putenv(const char *str){return _putenv(str);}
