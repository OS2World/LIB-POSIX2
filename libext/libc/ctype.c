/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
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

#define _ANSI_LIBRARY
#include <ctype.h>

#undef isalnum
int
isalnum(c)
	int c;
{
	return((_ctype + 1)[c] & (_UPPER|_LOWER |_DIGIT));
}

#undef isalpha
int
isalpha(c)
	int c;
{
	return((_ctype + 1)[c] & (_UPPER|_LOWER));
}

#undef isblank
int
isblank(c)
	int c;
{
	return(c == ' ' || c == '\t');
}

#undef iscntrl
int
iscntrl(c)
	int c;
{
	return((_ctype + 1)[c] & _CNTRL);
}

#undef isdigit
int
isdigit(c)
	int c;
{
	return((_ctype + 1)[c] & _DIGIT);
}

#undef isgraph
int
isgraph(c)
	int c;
{
	return((_ctype + 1)[c] & (_PUNCT|_UPPER|_LOWER|_DIGIT));
}

#undef islower
int
islower(c)
	int c;
{
	return((_ctype + 1)[c] & _LOWER);
}

#undef isprint
int
isprint(c)
	int c;
{
	return((_ctype+1)[c] & (_PRINT));
}

#undef ispunct
int
ispunct(c)
	int c;
{
	return((_ctype + 1)[c] & _PUNCT);
}

#undef isspace
int
isspace(c)
	int c;
{
	return((_ctype + 1)[c] & _SPACE);
}

#undef isupper
int
isupper(c)
	int c;
{
	return((_ctype + 1)[c] & _UPPER);
}

#undef isxdigit
int
isxdigit(c)
	int c;
{
	return((_ctype+1)[c] & (_XDIGIT));
}

#undef isascii
int
isascii(c)
	int c;
{
	return ((unsigned)(c) <= 0177);
}

#undef toascii
int
toascii(c)
	int c;
{
	return ((c) & 0177);
}

#undef _toupper
extern int _toupper (int _c) { return (_c-'a'+'A'); }

#undef _tolower
extern int _tolower (int _c) { return (_c-'A'+'a'); }

extern int toupper(int _c)  {return (islower(_c) ? _toupper(_c) : _c);}

extern int tolower(int _c)  {return (isupper(_c) ? _tolower(_c) : _c);}

