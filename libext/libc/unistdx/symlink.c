/**** symlink.c/cpfile.c: cpfile() alias symlink() -- emx-gcc **** 

 From: "mithra" <mithra@bga.com>
 To: emx@queue.iaehv.nl
 Subject: Re: COPY/MOVE system function?

 SInce I needed a copy I could use everywhere without invoking system(), 
 here is cpfile.c:

 ****  
 * Copy file from in_name to out_name (so don't have to make system call
 * on those systems that can't do so discretely [M$])
 *
 * return:
 * 0 if all's well
 * -1 on error. Errno is set accordingly.
 * Curtis W. Rendon 12/20/1999 initial
 *  modified 04/01/00 A. Hanses 
 *
 * hereby released to the public domain, please keep my comments,
 * no warranty inferred, implied, or specified.
 *  Curtis W. Rendon mithra@earthling.net  12/29/1999
 *
 ***** Prototype:  
extern int _cpFile(const char *in_name, const char *out_name);
 */
#include <stdio.h>

#ifdef VIOLATE_POSIX_USE_M$_EXTENSION
# include <io.h>          /* PC extension, not (Posix) portable */
#else
# include <unistd.h>        /* Emx has portable (Posix) headers */
#endif

#include <utime.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h> /* ANSI/ISO C forbids explicit declaration ! *
 It is a thread local modifiable (int) lvalue (usually a macro)  */

static __inline__ int _doTheCpy(FILE*const,FILE*);

static struct stat 	fileStatBuf;  /* File attributes buffers */
static struct utimbuf 	fileUtimeBuf;

	static __inline__ int 
_doTheCpy(FILE *const in_p,  FILE *out_p)
{	/* we assume pointers are valid and do not perform checks! */
	do {			/*  file i/o loop */
	 char buf[BUFSIZ];	/*  r/w no more than BUFSIZ at once     */
	 size_t count = 
		fread( (void*)buf, 1, sizeof(buf), in_p);    	/* read/write      *
								 * count bytes at once   */
		if ( ferror(in_p) ) { 
			errno = EIO; break;		/* Internal read error */
		}
		if (fwrite( (void*)buf, 1, count, out_p) < count) { 
			errno = EIO; break;		/* Internal write error */
		}
	} while (! feof(in_p));            /* end file i/o loop */

	if (fclose(out_p) == EOF || errno == EIO)
		return(-1);			 /* danger of data loss ! */
	if (fclose(in_p)   == EOF)		  return(-1);  
	return(0);
}

	extern int 
_cpFile(const char *in_name, const char *out_name)
{
	if (! in_name || ! out_name) { 
		errno = EFAULT; return(-1); 
	} else {
	 FILE *const in_p = 
		fopen(in_name, "rb");			/* point to in file */
		if (in_p == NULL) {
			_dPrintf("_cpFile(): Error opening in file [%s]\n", in_name);
			return(-1);           /* error opening in file */
		}
		if ((access(out_name, F_OK)) == 0) {	/* do not overwrite out file */
			if (fclose (in_p) == EOF) 	return(-1);       
			errno = EEXIST;
	 		return(-1); 
		} else { 
		 FILE *out_p = 
			fopen(out_name, "wb");   		/* pointer to new output file. *
						Input file attributes are also copied.      */
			if ((out_p ) == NULL) { /* error opening out file */
				_dPrintf("_cpFile(): Error opening out file [%s]\n", out_name);
				if (fclose (in_p) == EOF) 	return(-1); 
				return(-1); 
			}
			if ( _lstat(in_name, &fileStatBuf) ) {
				_dPrintf("_cpFile(): Error _lstat in file [%s]\n", in_name);
				return(-1);  			/* Input file stat() error  */
			}
			if ( _doTheCpy(in_p, out_p)  )
				return(-1);   			/* Error copying */
			fileUtimeBuf.actime  = fileStatBuf.st_atime;
			fileUtimeBuf.modtime = fileStatBuf.st_mtime;
			if ( utime(out_name, &fileUtimeBuf) )			return(-1);
			if ( chmod(out_name, fileStatBuf.st_mode) )		return(-1);
			return (0);
		}
	} /* not reached */
	fputs("\
libextensions: _cpFile(): Internal error: Fall through. Fix me!\n", stderr);
	errno = EFAULT; return(-1);  /* fall-through should not happen */
}

	extern int  /* Define _CPYFILE_FOR_SYMLINK to fake symlink() by copying */
_Link1(const char *in, const char *out) 
{
	if (!in || !out) { errno = EFAULT; return(-1); 
	} else {
	 FILE *const in_p = 
		fopen(in, "rb");  			/* point to in file */
		if (in_p == NULL) {
			_dPuts("Error opening in file");
			return(-1);           /* error opening in file */
		}
		if (access(out, F_OK) == 0) { /* symlink() does not overwrite out file */
			_dPuts("Error symlink() does not overwrite out file");
			if (fclose(in_p) == EOF) 		return(-1);       
			errno = EEXIST;
 			return(-1); 
		} else { 
		 FILE *out_p = 
			fopen(out, "wb");   		/* pointer to new output file. */
			if (out_p == NULL) { 		/* error opening out file */
				_dPuts("Error opening out file");
				if (fclose(in_p) == EOF) 	return(-1); 
				return(-1); 
			}
			fputs("\
libextensions: _Link1(): Faking a softlink with a copy. Expect problems!\n",
			 stderr);
			if ( _doTheCpy(in_p, out_p)  )
				return(-1);   			/* Error copying */
			return(0);
		}
	} /* not reached */
	fputs("\
libextensions: _Link1(): Internal error: Fall through. Fix me!\n", stderr);
	errno = EFAULT; return(-1);
}

	extern int 
_Link2(const char *in, const char *out)
{
	if (!in || !out) { 
		errno = EFAULT; return(-1); 
	} else {
		if (fopen(in, "rb") == NULL) return(-1);      /* error opening in file */
		if (access(out, F_OK) == 0) { /* symlink() does not overwrite out file */
			errno = EEXIST; 			return(-1); 
		} else { 
			if (fopen(out, "wb") == NULL)  		/* error opening out file */
				return(-1); 
			/* OS/2 does not natively support symlinks ! */
			errno = EPERM;
			return (-1);
		}
	}
	fputs("libextensions: _symlink(): Internal error: Fall through. Fix me!\n", stderr);
	errno = EFAULT; return(-1);  /* fall-through should not happen */
}

