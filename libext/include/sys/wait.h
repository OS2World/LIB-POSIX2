/* libExt+gcc: -- sys/wait.h
* $Header: /cvsroot/posix2/posix2/libext/include/sys/wait.h,v 1.1 2000/11/17 19:14:59 arnd Exp $
*/

#ifndef _SYS_WAIT_H
#define _SYS_WAIT_H

#if defined (__cplusplus)
extern "C" {
#endif

#define WNOHANG         1
#define WUNTRACED       2

#define WIFEXITED(st)   (((st) & 0x7f) == 0)
#define WIFSIGNALED(st) (((st) & 0x7f) != 0x7f && ((st) & 0x7f) != 0)
#define WIFSTOPPED(st)  (((st) & 0x7f) == 0x7f)

#define WEXITSTATUS(st) ((st) >> 8)
#define WTERMSIG(st)    ((st) & 0x7f)
#define WSTOPSIG(st)    ((st) >> 8)

#if !defined(_POSIX_SOURCE) && !defined(_PID_T)
#define _PID_T
typedef int pid_t;
#endif

extern pid_t wait(int*);
extern pid_t waitpid(int,int*,int);

#if !defined(_POSIX_SOURCE)||defined(_WITH_UNDERSCORE)
extern pid_t _wait(int*);
extern pid_t _waitpid(int,int*,int);
#endif

#if defined (__cplusplus)
}
#endif

#endif /* not SYS_WAIT_H*/
