/* strxtens.c (libext+gcc) -- non-Posix extensions to string.h */
/* Portions are: -- Copyright (c) 1996 by Eberhard Mattes, see Copying. */

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/nls.h>

  /* hackish workaround for missing dll exports from sys/nls.h; an emx bug */
static __const__ unsigned char 		/* DBCS-support broken. Fix me! */
_nls_ctype_tab_hackish_workaround[256] = {0};

	static __inline__ __const__ unsigned char  
__strx_isDbcsLead(__const__ unsigned int c)
{
	return(_nls_ctype_tab_hackish_workaround[c]);
}

/* Detect trailing slash or backslash for stat(), access(), and
   _tmpidxnam().  If MODE is 0, the trailing (back)slash may be
   preceded by an arbitrary character, including none.  Moreover, a
   trailing colon is treated like a trailing slash.  If MODE is 1, the
   trailing (back)slash must be preceded by a character which is not a
   (back)slash or a colon.  Note that the string pointed to by NAME
   need not be null-terminated! */ /* Support only SBCS and DBCS for now. */

	extern int 
_strHasTrailingSlash(__const__ char *str, __const__ size_t len,
							__const__ int mode)
{
  switch (mode) {
    case 0:
      if (len < 1 || (str[len-1] != '\\' && str[len-1] != '/'
                      && str[len-1] != ':'))
        return 0;
      if (len == 1 && (str[0] == '\\' || str[0] == '/')) return 1;
      break;
    case 1:
      if (len < 2 || (str[len-1] != '\\' && str[len-1] != '/'))
        return 0;
      break;

    default: return 0;
  }
  /* The str seems to have a trailing (back)slash.  There are two
     conditions which must _not_ hold:

     (1) the (back)slash is the 2nd byte of a DBCS character
     (2) the (back)slash is preceded by a SBCS (back)slash or a colon

     Condition (2) does not matter if MODE is 0.

          |         |     |     | return for MODE
     Case | str(*)  | (1) | (2) |   0   |   1
     -----+---------+-----+-----+-------+-------
     (a)  | ....//  | no  | yes |   1   |   0
     (b)  | ....S/  | no  | no  |   1   |   1
     (c)  | ...L2/  | no  | no  |   1   |   1
     (d)  | ....L/  | yes | -   |   0   |   0

     (*) S = SBCS character but not a (back)slash or colon
         L = lead byte
         2 = 2nd byte of DBCS char */
 { 
  size_t i = 0;
  do {
      if (__strx_isDbcsLead((unsigned char)str[i])) {
        if (i == len - 3) { /* Case (c) -- a DBCS character precedes
						                  the trailing (back)slash. */
          return 1;
        } else 
          if (i == len - 2) { /* Case (d) -- the last byte is the 2nd byte of a 
		            DBCS character and therefore is not a SBCS (back)slash. */
            return 0;
           } else
            i += 2;
      } else 
        if (i == len - 2) { 	/* We hit the character preceding the trailing 
             		(back)slash; this is either case (a) or (b).  Check for 
             			condition (2), condition (1) does not apply. */
          return(mode == 0||(str[i] != '\\'&&str[i] != '/'&&str[i] != ':'));
        }
        else  ++i;
  } while (1);
 }
}
	extern void 
_strRemoveTrailingSlashes(char*path) /* not constant !! */
{
  register size_t i = strlen(path) - 1;

  while ( i && (path[i] == '/'||path[i] == '\\') )
    { path[i] = '\0'; --i;}
}


/* External library versions */

	extern int 	/* Compare S and T, ignoring case. */
strcasecmp(__const__ char*s,__const__ char*t) {return _stricmp(s,t);}
	extern int /* Compare no more than N chars of S and T, ignoring case. */
memcasecmp(__const__ void*s,__const__ void*t,size_t n) {return _memicmp(s,t,n);}
	extern  int 
strncasecmp(__const__ char*s,__const__ char*t,size_t n){return _strnicmp(s,t,n);}
	extern long long 
strtoll(__const__ char*s,char**e,int r){return _strtoll(s,e,r);}
	extern unsigned long long 
strtoull(__const__ char*s,char**e,int r){return _strtoull(s,e,r);}
