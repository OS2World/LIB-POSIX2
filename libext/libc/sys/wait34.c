/* libExt+gcc: $Id: wait34.c,v 1.2 2000/11/25 16:33:36 arnd Exp $
 * Copyright (c) 2000 by A. Hanses
 */

#define INCL_DOSPROCESS
#include <os2.h>

#include <sys/resource.h>
#include <sys/errnox.h>

/* The following BSD interfaces are just a horrible kludge! FixMe! */

	extern pid_t 
_wait4(pid_t pid, int *status, int options, struct rusage *rusage)
{
  if (options != WNOHANG && options != WUNTRACED) 
      {errno = EINVAL; return (pid_t)-1;}

  if (pid == (pid_t)-1)
    pid = (pid_t)0;
  if (pid < (pid_t)-1) 			/* we do not have process groups */
    pid = __builtin_abs(pid);

 {
 APIRET rc;
 RESULTCODES res;
 PID pid2;

  if ((rc = DosWaitChild(DCWA_PROCESS, DCWW_WAIT, &res, &pid2, (PID)pid))) {
      errno = _rc2Errno(rc);
      return (pid_t)-1;
  }
  switch (res.codeTerminate) {

    case TC_EXIT:
   if (status != NULL) *status = res.codeResult << 8;
   if (rusage != NULL) {
      if (_getrusage(RUSAGE_CHILDREN, rusage))
          return (pid_t)-1;
   }
      break;

     case TC_HARDERROR:
     case TC_KILLPROCESS:
    if (status != NULL) *status = SIGTERM;
      break;

     default:
    if (status != NULL) *status = SIGSEGV;
      break;
  }
  return (pid_t)pid2;
 }
}

	extern pid_t 
wait4(pid_t p, int *s, int o, struct rusage *r) {return _wait4(p,s,o,r);}

	extern pid_t 
_wait3(int *s, int o, struct rusage *r) {return _wait4(-1,s,o,r);}

	extern pid_t 
wait3(int *s, int o, struct rusage *r) {return _wait4(-1,s,o,r);}

