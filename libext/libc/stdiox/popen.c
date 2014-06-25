/* popen.c -- popen/pclose for OS/2, originally by Glenn Gribble. */
/* $Id: popen.c,v 1.5 2000/11/25 16:33:35 arnd Exp $ */

#define INCL_DOSQUEUES
#define INCL_DOSPROCESS
#include <os2.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <unistd.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/errnox.h>

#if !(defined(__MT__) || defined(__ST_MT_ERRNO__))
#define DosEnterCritSec()	(0)
#define DosExitCritSec()	(0)
#endif

/* First, a singly linked-list to keep track of pipes */
	/* Map Handles onto termination codes. */
struct _hfPipe_llist {
  int hfPipe;	/* key: pipe handle to commincate with child */
  PID pid;    	/* val: pid of spawned child */
  struct _hfPipe_llist *prev; 
};
	/* The __popen_LL*() functions use it. */
static struct _hfPipe_llist *
hfPipeLL_p = (struct _hfPipe_llist*)NULL;

	static __inline__ struct _hfPipe_llist *
__popen_LLinsert(HFILE hfPipe, PID pid)
{
 APIRET rc;
 struct _hfPipe_llist *const
	new = (struct _hfPipe_llist*)malloc(sizeof(struct _hfPipe_llist));
	if (!new)
		return(new); /* malloc sets errno */

	new->hfPipe  = hfPipe;
	new->pid   = pid;

	rc = DosEnterCritSec();
	if (rc) {
		DosExitCritSec();
		errno = _rc2Errno(rc);
		return (struct _hfPipe_llist*)NULL;
	}
	new->prev = hfPipeLL_p; /* we point to the previously first entry */

	hfPipeLL_p = new; /* Now we are the first entry in the linked list */

	rc = DosExitCritSec();
	if (rc) {
		errno = _rc2Errno(rc);
		return (struct _hfPipe_llist*)NULL;
	}
	return(new);
}
	static __inline__ void
__popen_LLdelete(int hfPipe)
{
 APIRET
	rc = DosEnterCritSec();
	if (rc) {
		DosExitCritSec();
		_dPrintf("\
libext: __popen_LLdelete(): DosEnterCritSec() returns error # %lu", rc);
		errno = _rc2Errno(rc);
		return;
	} {
	struct _hfPipe_llist *this = hfPipeLL_p; /* initialize to first entry */
	struct _hfPipe_llist *last = (struct _hfPipe_llist*)NULL;

	while (this) {
		if (this->hfPipe != hfPipe) {
		/* try the next one. */
			last = this;
			this = this->prev;
			continue;
		}
		break;
	}
	if (!this) {
		DosExitCritSec();
		_dPuts("libext: __popen_LLdelete(): Error: No entry found!\n");
		return;
	}
	/* Delete this node and leave. */
	if (last) {
		last->prev = this->prev; 
		_dPuts("libext: __popen_LLdelete(): restore linkage...\n\n");
	}
	else { 	/* deleted first entry */
		_dPuts("libext: __popen_LLdelete(): deleted first entry...\n\n");
		hfPipeLL_p = this->prev; /* point to start of list */
	}
	rc = DosExitCritSec();
	if (rc) {
		_dPrintf("\
libext: __popen_LLdelete(): DosExitCritSec() returns error # %lu", rc);
		errno = _rc2Errno(rc);
		return;
	}
	free(this);
	}
	return;
}
	static inline PID
__popen_LLlookup(int hfPipe)
{
 struct _hfPipe_llist *this;
 APIRET
	rc = DosEnterCritSec();
	if (rc) {
		DosExitCritSec();
		_dPrintf("\
libext: __popen_LLdelete(): DosEnterCritSec() returns error # %lu", rc);
		errno = _rc2Errno(rc);
		return(0);
	}
	this = hfPipeLL_p;
  while (this) {
    if (this->hfPipe == hfPipe) {
			DosExitCritSec();
      return(this->pid);
		}
    this = this->prev;
  }
	rc = DosExitCritSec();
	if (rc) {
		_dPrintf("\
libext: __popen_LLdelete(): DosExitCritSec() returns error # %lu", rc);
		errno = _rc2Errno(rc);
	}
  /* Zero is special in this context anyway. */
  return(0); /* not found */
}

#ifdef DEBUG
	extern ULONG
__popen_LLlength(void)
{
  struct _hfPipe_llist *this = hfPipeLL_p;
  size_t len;

  for (len = 0; this; ++len)
    this = this->prev;

  return len;
}
	extern ULONG
__popen_LLprint(void)
{
  struct _hfPipe_llist *this = hfPipeLL_p;
  size_t i;

  for (i = 0; this; ++i) {
      printf ("hfPipe_ll[%ld] == (%5d --> %5ld)\n",
			  i, this->hfPipe, this->pid);
      this = this->prev;
  }
  if (i == 0)
    printf ("No entries.\n");

  return i;
}
#endif

/*
 *  Routine: popen
 *  Returns: FILE pointer to pipe.
 *  Action : Exec program connected via pipe, connect a FILE * to the
 *           pipe and return it.
 *  Params : Command - Program to run
 *           Mode    - Mode to open pipe.  "r" implies pipe is connected
 *                     to the programs stdout, "w" connects to stdin.
 */
	extern FILE *
_popen(const char *Command, const char *Mode)
{
 HFILE End1, /* parent's pipe handle */
       End2, /* child's pipe handle */
       Old,  /* save original stdin ot -out */
       Tmp, /* tmporary buffer for handles */
       StdIO;  /* new redirected stdin ot -out */
 APIRET rc =
	DosCreatePipe(&End1, &End2, PIPE_BUF);
		if (rc) {errno = _rc2Errno(rc); return (FILE*)NULL;}
 
	StdIO = (Mode[0] == 'w') ? STDIN_FILENO : 
 		 ((Mode[0] == 'r') ? STDOUT_FILENO : 0xffffffff);

	if (StdIO == 0xffffffff) {
		errno = EINVAL;
		return (FILE*)NULL;
	}
	if (StdIO == STDIN_FILENO) {
		Tmp = End1; End1 = End2; End2 = Tmp;
	}

  fflush(stderr);
	fflush(stdout);
	Old = 0xffffffff; /* save stdin or stdout to Old */
	rc = DosDupHandle(StdIO, &Old);
		if (rc) {errno = _rc2Errno(rc); return (FILE*)NULL;}
	rc = DosSetFHState(Old, OPEN_FLAGS_NOINHERIT);
		if (rc) {errno = _rc2Errno(rc); return (FILE*)NULL;}

	Tmp = StdIO; /* redirect stdin or stdout to End2 */
	rc = DosDupHandle(End2, &Tmp);
		if (rc) {errno = _rc2Errno(rc); return (FILE*)NULL;}
	rc = DosClose(End2);
		if (rc) {errno = _rc2Errno(rc); return (FILE*)NULL;}
	rc = DosSetFHState(End1, OPEN_FLAGS_NOINHERIT);
		if (rc) {errno = _rc2Errno(rc); return (FILE*)NULL;} /* Make only the 
   relevant reading end / writing end inheritable.
   Why does inheritability matter?  Consider the case of a
   pipe carrying data from the parent process to the child
   process.  The child wants to read data from the parent until
   it reaches the EOF.  Now, the only way to send an EOF on a pipe
   is to close all the handles to its writing end.  Obviously, the 
   parent has a handle to the writing end when it creates the child.
   If the child inherits this handle, then it will never close it
   (the child has no idea it's inherited it), and will thus never
   receive an EOF on the pipe because it's holding a handle
   to it. In Unix, the forked child process closes the pipe ends before it
   execs. In OS/2 and NT, you create the pipe with uninheritable 
   handles, and then make the appropriate ends inheritable.  */
	
 { /* Construct the argument vector of two zero-terminated strings */
 char CmdLine[MAXPATHLEN], *CmdLine_p;
 RESULTCODES Result;
 FILE* File;
 const char *CmdExe = getenv("EMXSHELL");

	_bzero(CmdLine, sizeof(CmdLine));
	if (CmdExe == NULL)
		if ((CmdExe = getenv("COMSPEC")) == NULL) 
				CmdExe = "cmd.exe";
	strcpy(CmdLine, CmdExe);

	if (_rindex(CmdExe, '/'))
		CmdLine_p = _strdup(_rindex(CmdExe, '/'));
	else
		CmdLine_p = _strdup(_rindex(CmdExe, '\\'));

	if (!CmdLine_p)
			CmdLine_p = _strdup(&CmdExe[0]);

	if (! CmdLine_p )
		return (FILE*)NULL;
	_strlwr(CmdLine_p);
		
	if (strstr(CmdLine_p, "cmd.exe") || strstr(CmdLine_p, "4os2.exe"))
		strcat(&CmdLine[0] + strlen(CmdLine) + 1, " /c "); /* terminate string */
	else
		strcat(&CmdLine[0] + strlen(CmdLine) + 1, " -c ");
	free(CmdLine_p);
	CmdLine_p = &CmdLine[0] + strlen(CmdLine) + 5;	/* point to end of vector */
	strcpy(CmdLine_p, Command);		/* append rest of argument string to it */

	_dPrintf("libext: popen(): Executing %s ", CmdLine);
	_dPrintf("%s ...\n", CmdLine + strlen(CmdLine) + 1);

	rc = DosExecPgm(CmdLine, MAXPATHLEN, EXEC_ASYNCRESULT, 
				CmdLine, 0, &Result, CmdExe);
	Tmp = StdIO; /* restore stdin or stdout */
	if (DosDupHandle(Old, &Tmp)
			|| DosClose(Old)) {
		_dPuts("libext: popen(): DosDupHandle() or DosClose() error!");
		return (FILE*)NULL;
	}

	if (rc) {
		DosClose(End1);
		_dPrintf("libext: popen(): DosExecPgm(): rc = %lu... \n", rc);
		return (FILE*)NULL;
	}
	/* Emx does not allow mixing operating system handles and
	 * C library handles, so we have to convert; errno is set 
	 * accordingly by the conversion functions.
	 */
	End1 = _imphandle((int)End1);
	if (End1 == (ULONG)-1 || ! 
		__popen_LLinsert(End1, Result.codeTerminate) )
		return (FILE*)NULL;
		
	File = _fdopen((int)End1, Mode);
	return(File);
 }
}

/*
 *  Routine: popenRW
 *  Returns: PID of child process
 *  Action : Exec program connected via pipe, connect int fd's to 
 *           both the stdin and stdout of the process.
 *  Params : ARGV  - Commandline to run
 *           Pipes - Array of 2 ints to store the pipe descriptors
 *                     Pipes[0] writes to child's stdin,
 *                     Pipes[1] reads from child's stdout/stderr
 */
	extern int
_popenRW(const char *const argv[2], int *pipes, int mode)
{
 HFILE Out1, Out2, In1, In2; /* read and write handles for 2 pipes */
 HFILE OldStdin = 0xffffffff, OldStdout = 0xffffffff, Tmp;
 APIRET rc =
    DosCreatePipe(&Out2, &Out1, PIPE_BUF); /* child writes to Out1 */
    if (rc) {
      DosClose(Out1);
      DosClose(Out2);
      errno = _rc2Errno(rc); return(-1);
    }
    rc = DosCreatePipe(&In1, &In2, PIPE_BUF); /* child reads from In1 */
    if (rc) {
        DosClose(In1);
        DosClose(In2);
        errno = _rc2Errno(rc); return(-1);
    }
    /* Save and redirect std{in,out}, do not inherit saved duplicates */
    do {
    	rc = DosDupHandle(STDIN_FILENO, &OldStdin);
    	if (rc) 
    	  break;
    	rc = DosSetFHState(OldStdin, OPEN_FLAGS_NOINHERIT);
    	if (rc) 
    	  break;
    	rc = DosDupHandle(STDOUT_FILENO, &OldStdout);
    	if (rc) 
    	  break;
    	rc = DosSetFHState(OldStdout, OPEN_FLAGS_NOINHERIT); 
    } while (0);
    
    if (rc) {
    	DosClose(OldStdin);
    	DosClose(OldStdout);
    	_dPrintf("rc = %lu",rc); 
    	errno = _rc2Errno(rc); 
    	return(-1);
    }
    fflush(stderr);
    fflush(stdout);
    Tmp = STDIN_FILENO; 	/* Redirect std{in,out} */
    rc = DosDupHandle(In1, &Tmp); /* connect In1 to stdin of child */
    if (rc) {
    	DosClose(In1);
    	DosClose(Tmp);
    	errno = _rc2Errno(rc); return(-1);
    }
    Tmp = STDOUT_FILENO;
    rc = DosDupHandle(Out1, &Tmp); /* connect Out1 to stdout of child */
    if (rc) {
    	DosClose(Out1);
    	DosClose(Tmp);
    	errno = _rc2Errno(rc); return(-1);
    }
    /* Close/mask original pipe handles not needed */
    DosClose(In1);
    DosClose(Out1);
    if ( (rc = DosSetFHState(In2,  OPEN_FLAGS_NOINHERIT))
    	|| (rc = DosSetFHState(Out2, OPEN_FLAGS_NOINHERIT)) ) {
    	DosClose(In2);
    	DosClose(Out2);
    	errno = _rc2Errno(rc); return(-1);
    }
    /* Spawn we now our hoary brood. */
    {RESULTCODES Result; char buf[MAXPATHLEN];

    _bzero(buf, sizeof(buf));
    strcpy(buf, argv[1]);

    rc = DosExecPgm(buf,MAXPATHLEN,EXEC_ASYNCRESULT,buf,0,&Result,argv[0]);

    /* Restore std{in,out} */
    Tmp = STDIN_FILENO;
    if (DosDupHandle(OldStdin, &Tmp) ||
    		DosClose(OldStdin)) {
    	DosClose(Tmp);
    	errno = EMFILE; return(-1);
    }
    Tmp = STDOUT_FILENO;
    if (DosDupHandle (OldStdout, &Tmp) ||
    		DosClose (OldStdout)) {
    	DosClose(Tmp);
    	errno = EMFILE;  return(-1);
    }
    if (rc) {
        DosClose(In2);
        DosClose(Out2);
        errno = _rc2Errno(rc); return(-1);
    }
    pipes[0] = _imphandle((int)In2); 	/* this pipe connects to child's stdin */
    if (_setmode(pipes[0], mode) == (-1))
    	return(-1);
    pipes[1] = _imphandle((int)Out2); 	/* this pipe connects to stdout */
    if (_setmode(pipes[1], mode) == (-1))
    	return(-1);

    /* Save ID of write-to-child pipe for pclose() */
    __popen_LLinsert(In2, Result.codeTerminate);
    
    return(Result.codeTerminate);
  }
}


/*
 *  Routine: pclose
 *  Returns: -1 on error
 *  Action : Close a pipe opened with popen();
 *  Params : Pipe - pipe to close
 */
	extern int
_pclose(FILE *Pipe) 
{
 const int Handle = fileno(Pipe);
 PID pid = __popen_LLlookup(Handle);

    if (fclose(Pipe)) {
    	perror("libext: _pclose(): Error");
    	return(-1);
    }

    /* if pid is zero, something's seriously wrong */
    if (pid != 0) {
     PID pidEnded;
     RESULTCODES res = {(ULONG)-1, (ULONG)-1};
     APIRET rc =
      DosWaitChild(DCWA_PROCESSTREE, DCWW_WAIT, &res, &pidEnded, pid);
      	if (rc) {errno = _rc2Errno(rc); return(-1);}

      __popen_LLdelete(Handle);

      return(res.codeTerminate == 0 ? (int)res.codeResult : (-1));
    } 
   	errno = EBADF;
   	return(-1);
}

	extern FILE * 
popen(const char*Command, const char*Mode) { return _popen(Command, Mode);}

	extern int
pclose(FILE *Pipe) { return _pclose(Pipe);}

