/*  rename.c (libExt) --
 *  $Id: rename.c,v 1.9 2000/08/22 22:13:18 amai Exp $
 *
 *  Provide a "better" rename than EMX.
 *  See below for more comments on this issue.
 */

#define INCL_DOSFILEMGR   /* File Manager values */
#define INCL_DOSERRORS    /* Error values */
#include <os2.h>
#include <stdio.h>
#include <libgen.h>       /* basename() */
#include <sys/param.h>
#include <errno.h>
#include <sys/errnox.h>   /* we need a looong list of errnos; use some libExt stuff ... */

#ifndef TRUE
#define TRUE  (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif
/* used by FileExists */
#define FOUND     TRUE
#define NOTFOUND  FALSE


/* Prototypes for local stuff */
static int    FileExists(const char *name);
static ULONG  FileType(const char *name);
static int    IsDir(const char *name);
static int    IsReadOnly(const char *name);


static int
FileExists(const char *name) {


    /* CASE-SENSITIVE check whether a fle <name> exists.
       Returns FOUND/1 if a "dir"-like command would report file <name>
       to exist without using a non-trivial file mask.
       The "dir" reference is not really precise; check the code to
       see on which OS/2 API our call relies ...
     */

     HDIR          hdirFindHandle = HDIR_CREATE;
     FILEFINDBUF3  FindBuffer     = {0};
     ULONG         ulResultBufLen = sizeof(FILEFINDBUF3);
     ULONG         ulFindCount    = 1;        /* Look for 1 file at a time    */
     APIRET        arc            = 0;
 
     arc = DosFindFirst(name,                 /* File pattern - all files     */
                        &hdirFindHandle,      /* Directory search handle      */
                        FILE_NORMAL,          /* Search attribute             */
                        &FindBuffer,          /* Result buffer                */
                        ulResultBufLen,       /* Result buffer length         */
                        &ulFindCount,         /* Number of entries to find    */
                        FIL_STANDARD);        /* Return Level 1 file info     */

     DosFindClose(hdirFindHandle);    /* Close directory handle */
 
     if (arc != NO_ERROR) {
        return NOTFOUND;
     } else {
       /* we only compare the basename()s !? */
       if (strcmp(basename(name), FindBuffer.achName))
          return NOTFOUND;
       else
          return FOUND;
     } /* endif */
  }

/* Reading the docs I think we can use this: this mask should be of
   bits which are always zero?! */
#define FILE_ERRORMASK 0xFF00


/*
 Now we need some utility functions; since rename() shouldn't depend
 on other similar libc stuff, we use Dos*().
 We need them to supply the required functionality of rename(2) which
 is beyond the simple mapping to DosMove().
 */



static inline ULONG
FileType(const char *name) {
    /* Somehow the lean OS/2 version of "stat()" ...:
       return only the file attribute bits (ULONG) */
  APIRET arc;
  FILESTATUS3 filestatus3Info;

  arc = DosQueryPathInfo(name,
                         FIL_STANDARD,
                         (PFILESTATUS3)&filestatus3Info,
        	         sizeof(filestatus3Info));
  if (arc)
        /* error */
        return (ULONG)FILE_ERRORMASK; /* this should be unique ... */
  else
      return filestatus3Info.attrFile;
}


static inline int
IsDir(const char *name) {

    /* Return TRUE if recognized as a directory by Dos*() */

    ULONG filetyperc;

    filetyperc = FileType(name);
    if (filetyperc&FILE_DIRECTORY)
         return (int)TRUE;
    else
         return (int)FALSE;
}


static inline int
IsReadOnly(const char *name) {

    /* Return TRUE if ReadOnly-FILE by Dos*() */

    ULONG filetyperc;

    filetyperc = FileType(name);
    if (filetyperc&FILE_READONLY)
         return (int)TRUE;
    else
         return (int)FALSE;
}



extern int
_rename (const char *oldname, const char *newname)
{

/* short note: the numerous return statements are used by
   intention: simple counting of statements was in favour
   doing so to speed things up. Due to the IO nature of this
   API one won't notice it however?! */

/* Those values are fixed by standard, but for readibility ... */
#define SUCCESS (0)
#define FAILURE (-1)

  APIRET arc;

  arc = DosMove(oldname, newname); /* just try */
  if (arc==0)
    return SUCCESS;

  /* Something didn't work out ... */
  /*  fprintf(stderr, "rc(DosMove) = %i\n", rc); */
  if (arc==ERROR_ACCESS_DENIED) {
      /* It might be that the target file <newname> already exists.
         (Stefan's) idea is to check whether a file blocks which name
         matches case-sensitively the <newname>. */
      /* fprintf(stderr, "ERROR_ACCESS_DENIED\n"); */
      if (FileExists(newname)==FOUND) {
          arc = DosDelete(newname);
          if (arc==0) {
             arc = DosMove(oldname, newname);
             if (arc==0) {
                 return SUCCESS;
             } else {
                 /* DosMove() failed, get errno right?! */
                 /* fprintf(stderr, "rc(DosMove) = %i\n", rc); */
                  errno = _rc2Errno(arc);
                  return FAILURE;
             }
           } else  {
           /* DosDelete() failed:
              Bad luck - give up:
              file might be "open", write protected, ... ?
              Get errno right?!  */
              errno = _rc2Errno(arc);
              return FAILURE;
           }
      } /* FileExists(newname) */
      else {
        /* Hmm, ERROR_ACCESS_DENIED but not FileExists(ewname):
         * ?
         */
          errno = _rc2Errno(arc);
          return FAILURE;
      }
  } 
  else {
    /* an error other than that ERROR_ACCESS_DENIED */
     errno = _rc2Errno(arc);
     return FAILURE;
  }

}


extern int
rename (const char *oldname, const char *newname)
{
  return _rename (oldname, newname);
}
