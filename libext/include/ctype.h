/* libExt+gcc: $Id: ctype.h,v 1.2 2000/05/17 08:09:18 veit Exp $ */

#ifndef _CTYPE_H
#define _CTYPE_H

#if defined (__cplusplus)
# ifndef __inline__ 
#  define __inline__ inline
#  define __const__  const
# endif
extern "C" {
#endif

extern __const__ unsigned char _ctype[];

#define _UPPER  0x01
#define _LOWER  0x02
#define _DIGIT  0x04
#define _XDIGIT 0x08
#define _CNTRL  0x10
#define _SPACE  0x20
#define _PUNCT  0x40
#define _PRINT  0x80

extern int isalnum(int);
extern int isalpha(int);
extern int iscntrl(int);
extern int isdigit(int);
extern int isgraph(int);
extern int islower(int);
extern int isprint(int);
extern int ispunct(int);
extern int isspace(int);
extern int isupper(int);
extern int isxdigit(int);
extern int tolower(int);
extern int toupper(int);

#if !defined(__STRICT_ANSI__) && !defined(_POSIX_SOURCE)
extern int isblank(int);
extern int isascii(int);
extern int toascii(int);
extern int _tolower(int);
extern int _toupper(int);
#endif

#define isalnum(c)  ((_ctype+1)[(int)c] & (_UPPER|_LOWER|_DIGIT))
#define isalpha(c)  ((_ctype+1)[(int)c] & (_UPPER|_LOWER))
#define iscntrl(c)  ((_ctype+1)[(int)c] & (_CNTRL))
#define isdigit(c)  ((_ctype+1)[(int)c] & (_DIGIT))
#define isgraph(c)  ((_ctype+1)[(int)c] & (_PUNCT|_UPPER|_LOWER|_DIGIT))
#define islower(c)  ((_ctype+1)[(int)c] & (_LOWER))
#define isprint(c)  ((_ctype+1)[(int)c] & (_PRINT))
#define ispunct(c)  ((_ctype+1)[(int)c] & (_PUNCT))
#define isspace(c)  ((_ctype+1)[(int)c] & (_SPACE))
#define isupper(c)  ((_ctype+1)[(int)c] & (_UPPER))
#define isxdigit(c) ((_ctype+1)[(int)c] & (_XDIGIT))

int toupper (int);
int tolower (int);
int _toupper (int);
int _tolower (int);

#if !defined (_CTYPE_FUN)
extern __inline__ int _toupper (int _c) { return (_c-'a'+'A'); }
extern __inline__ int _tolower (int _c) { return (_c-'A'+'a'); }
extern __inline__ int toupper(int _c)
  {return (islower(_c) ? _toupper(_c) : _c);}
extern __inline__ int tolower(int _c)
  {return (isupper(_c) ? _tolower(_c) : _c);}
#endif

#if !defined (__STRICT_ANSI__) && !defined (_POSIX_SOURCE)

#define isascii(c)  ((unsigned)(c) <= 0x7f)
#define toascii(c)  ((c) & 0x7f)

#endif


#if defined (__cplusplus)
}
#endif

#endif /* not _CTYPE_H */
