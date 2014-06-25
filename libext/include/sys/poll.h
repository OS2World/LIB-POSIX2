/* sys/poll.h (libext+gcc) - $Id: poll.h,v 1.4 2000/04/18 12:01:52 veit Exp $
   This is Public Domain   written in 2000 by
   Alexander Mai   <st002279@hrzpub.tu-darmstadt.de >
   */
#ifndef _SYS_POLL_H
#define _SYS_POLL_H

#if defined(__cplusplus)
# ifndef __inline__ 
#  define __inline__ inline
# endif
extern "C" {
#endif

/* this is not available in "all" implementations, OpenGroup tells about it */
typedef unsigned int nfds_t;

/* data structure for poll request data */
struct pollfd {
   int fd;          /* desriptor */
   short events;    /* what to look for */
   short revents;   /* what really happened */
};

/* the function prototype */
int poll(struct pollfd ufds[], nfds_t nfds, int timeout);
int _poll(struct pollfd ufds[], nfds_t nfds, int timeout);

/* values for the (r)events bitmask */
#define POLLIN       0x0001
#define POLLPRI      0x0002
#define POLLOUT      0x0004
#define POLLERR      0x0008
#define POLLHUP      0x0010
#define POLLNVAL     0x0020
#define POLLRDNORM   0x0040
#define POLLRDBAND   0x0080
       /* see OpenGroup docs: */
#define POLLWRNORM   POLLIN 
#define POLLWRBAND   0x0100

#if defined (__cplusplus)
}
#endif

#endif /* _SYS_POLL_H */
