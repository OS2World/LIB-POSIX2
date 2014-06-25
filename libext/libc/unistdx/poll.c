/* $Id: poll.c,v 1.2 2000/08/14 16:13:42 amai Exp $ -- (libext + gcc)
   This is Public Domain written in 2000 by
   Alexander Mai <st002279@hrzpub.tu-darmstadt.de> 
 */
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <errno.h>

#include <sys/poll.h> /* our public header */

#define MILLISECS 1000
#define MICROSECS (1000*MILLISECS)

	extern int 
_poll(struct pollfd ufds[], nfds_t nfds, int timeout)
{
 int maxfd = (-1);   		/* select() stuff */
 fd_set rfds, wfds, efds;
 struct timeval tv;

 unsigned int i;   		/* internal stuff */
 /* int supported = -1; */
 struct pollfd *ufd;
 int selectrc;


#if 0
  Hmmmm, OPEN_MAX ? ?
    if (nfds > OPEN_MAX) {
      errno = EINVAL;		/* greetings go to Arnd! ;-) */
      return (-1);
    }
#else
  /* ?! */
  if (nfds >= FD_SETSIZE) {
      errno = EINVAL;
      return (-1);
  }
#endif

  /* prepare select() input */
  FD_ZERO (&rfds);
  FD_ZERO (&wfds);
  FD_ZERO (&efds);	/* EMX docs state that the only events
 	that do set bits here are "pending errors for sockets" */

  if (0 == timeout) {
      /* return immediately */
      tv.tv_sec = 0;
      tv.tv_usec = 0;
  } else 
  if (0 > timeout) {
      /* Hmm, it may be common to use any negative value here as a 
         synonym for (-1)
         Wait until event occurs or call is interrupted!
       */
  } else {   /* ok, a valid, finite timeout was given */
      /* I feel tired while writing, I hope I converted in the
         right direction ... ;-) */
      tv.tv_sec = timeout / MILLISECS;
      tv.tv_usec = (timeout - tv.tv_sec * MILLISECS) * 1000;
  }
  /* ok, cycle through the given list and try to prevent crashes,
     even for bogus arguments */
  ufd = ufds;
  for (i = 0; i < nfds; i++) {
      if (!ufd)	{
	  errno = EINVAL;
	  return -1;
      }
      maxfd = (ufd->fd > maxfd) ? ufd->fd : maxfd;
      if (ufd->events & POLLIN)	{
	  FD_SET (ufd->fd, &rfds);
	  FD_SET (ufd->fd, &efds);
      }
      if (ufd->events & POLLOUT) {
	  FD_SET (ufd->fd, &wfds);
	  FD_SET (ufd->fd, &efds);
      }
      if (ufd->events & POLLRDBAND) {
	  fprintf (stderr, "poll(): POLLRDBAND is not supported!\n");
	  errno = EINVAL;  return (-1);
      }
      if (ufd->events & POLLPRI) {
	  fprintf (stderr, "poll(): POLLPRI is not supported!\n");
	  errno = EINVAL;  return (-1);
      }	/* POLLHUP, POLLERR and POLLNVAL are ignored */
      /* clear revents; later only the matching will be set */
      ufd->revents = (short int) 0;
      ufd++;
    }

  /* Do the real work now and call select() */
  if (timeout >= 0) {
      selectrc = select (maxfd + 1, &rfds, &wfds, &efds, &tv);
  } else 
      selectrc = select (maxfd + 1, &rfds, &wfds, &efds, NULL);

  if (-1 == selectrc) {      /* an error has occured */
      /* perror("select() from within poll()");
         might give:
         - "Bad file number"
         - ...
         Hmm, we should make this list rather complete and
         then see what to do??
         Unless we do something like multiple select calls
         or our own analysis we have to figure out "manually"
         which handles contributed to the error and set
         the revents in the according structure ufds:
         e.g. a loop over nfds and fcntl(, F_GETFD) ??
       */
      ufd = ufds;
      for (i = 0; i < nfds; i++) {
	  int frc;
	  frc = fcntl (ufd->fd, F_GETFD);
	  if (-1 == frc) {
	      if (EBADF == errno) { 		/* found a culprit! */
		  ufd->revents |= POLLNVAL;
	      }
	  } /* do check whether nfds==1 ??  POLLERR, POLLHUP ? */
	  ufd++;
      }
      errno = EINVAL;		/* ?? */
      return (-1);
    } else
    if (0 == selectrc) { /* time-out */
      return 0;
    } else { 		/* Check what has happened.
        No, I did not attempt to write an optimized routine
        here, we do not notice when all raised bits are assigned, etc. */
      ufd = ufds;
      for (i = 0; i < nfds; i++) {
	  if (ufd->events & POLLIN)
	    if (FD_ISSET (ufd->fd, &rfds)) ufd->revents |= POLLIN;
	  if (ufd->events & POLLOUT)
	    if (FD_ISSET (ufd->fd, &wfds)) ufd->revents |= POLLOUT;
    }
    return(selectrc);
  }
}
	extern int 
poll(struct pollfd u[],nfds_t n,int t){return(_poll(u,n,t));}
