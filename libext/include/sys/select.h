/* sys/select.h (libExt+gcc): $Id: select.h,v 1.1 2000/05/15 09:47:47 veit Exp $ */

#ifndef _SYS_SELECT_H
#define _SYS_SELECT_H

#if defined (__cplusplus)
extern "C" {
#endif

#if !defined (FD_SETSIZE)
#define	FD_SETSIZE      256
#elif FD_SETSIZE < 256
#error FD_SETSIZE must be at least 256
#endif

#if !defined (_FD_SET_T)
#define _FD_SET_T
typedef unsigned long fd_mask;
#define NFDBITS	(sizeof(fd_mask) * NBBY)	/* bits per mask */

#ifndef howmany
#define	howmany(x, y)	(((x) + ((y) - 1)) / (y))
#endif

typedef	struct _fd_set {
	fd_mask	fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} fd_set;
#endif

#ifndef FD_SET
#define	FD_SET(n,s)    ((s)->fds_bits[(n)/32] |=  (1L << ((n) & 0x1f)))
#define	FD_CLR(n,s)    ((s)->fds_bits[(n)/32] &= ~(1L << ((n) & 0x1f)))
#define	FD_ISSET(n,s)  ((s)->fds_bits[(n)/32] &   (1L << ((n) & 0x1f)))
#define FD_ZERO(s)     (void)memset (s, 0, sizeof (*(s)))
#endif

struct timeval;

int select (int, struct _fd_set *, struct _fd_set *, struct _fd_set *,
    struct timeval *);

int _select (int, struct _fd_set *, struct _fd_set *, struct _fd_set *,
    struct timeval *);

void *memset (void *, int, size_t); /* Used by FD_ZERO */

#if defined (__cplusplus)
}
#endif

#endif /* not _SYS_SELECT_H */
