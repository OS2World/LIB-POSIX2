/* Berkeley sys/socket.h - Modified for emx by hv and em 1994-1997 */
/* $Id: socket.h,v 1.3 2000/04/03 18:16:13 veit Exp $ */

/*
 * Copyright (c) 1982,1985,1986,1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	from: @(#)socket.h	7.13 (Berkeley) 4/20/91
 */

#ifndef _SYS_SOCKET_H_
#define _SYS_SOCKET_H_

#if !defined (_SYS_TYPES_H)
#warning <sys/socket.h> requires <sys/types.h>
#include <sys/types.h>
#endif

#include <sys/uio.h>

#if defined (__cplusplus)
extern "C" {
#endif

#ifndef _EMX_TCPIP
#define _EMX_TCPIP
#endif

/*
 * Definitions related to sockets: types, address families, options.
 */

/*
 * Types
 */
#define	SOCK_STREAM	1		/* stream socket */
#define	SOCK_DGRAM	2		/* datagram socket */
#define	SOCK_RAW	3		/* raw-protocol interface */
#define	SOCK_RDM	4		/* reliably-delivered message */
#define	SOCK_SEQPACKET	5		/* sequenced packet stream */

/*
 * Option flags per-socket.
 */
#define	SO_DEBUG	0x0001		/* turn on debugging info recording */
#define	SO_ACCEPTCONN	0x0002		/* socket has had listen() */
#define	SO_REUSEADDR	0x0004		/* allow local address reuse */
#define	SO_KEEPALIVE	0x0008		/* keep connections alive */
#define	SO_DONTROUTE	0x0010		/* just use interface addresses */
#define	SO_BROADCAST	0x0020		/* permit sending of broadcast msgs */
#define	SO_USELOOPBACK	0x0040		/* bypass hardware when possible */
#define	SO_LINGER	0x0080		/* linger on close if data present */
#define	SO_OOBINLINE	0x0100		/* leave received OOB data in line */
#define	SO_L_BROADCAST	0x0200		/* limited broadcast sent on all IFs*/
#define	SO_RCV_SHUTDOWN	0x0400		/* set if shut down called for rcv */
#define	SO_SND_SHUTDOWN	0x0800		/* set if shutdown called for send */

/*
 * Additional options, not kept in so_options.
 */
#define SO_SNDBUF	0x1001		/* send buffer size */
#define SO_RCVBUF	0x1002		/* receive buffer size */
#define SO_SNDLOWAT	0x1003		/* send low-water mark */
#define SO_RCVLOWAT	0x1004		/* receive low-water mark */
#define SO_SNDTIMEO	0x1005		/* send timeout */
#define SO_RCVTIMEO	0x1006		/* receive timeout */
#define	SO_ERROR	0x1007		/* get error status and clear */
#define	SO_TYPE		0x1008		/* get socket type */
/* #define	SO_OPTIONS	0x1010		get socket options */

/*
 * Structure used for manipulating linger option.
 */
struct	linger {
#ifdef __RSXNT__
	short	l_onoff;		/* option on/off */
	short	l_linger;		/* linger time */
#else
	int	l_onoff;		/* option on/off */
	int	l_linger;		/* linger time */
#endif
};

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define	SOL_SOCKET	0xffff		/* options for socket level */

/*
 * Address families.
 */
#define	AF_UNSPEC	0		/* unspecified */
#define	AF_LOCAL	1		/* local to host (pipes, portals) */
#define	AF_UNIX		AF_LOCAL	/* backward compatibility */
#define	AF_OS2		AF_UNIX		/* backward compatibility */
#define	AF_INET		2		/* internetwork: UDP, TCP, etc. */
#define	AF_IMPLINK	3		/* arpanet imp addresses */
#define	AF_PUP		4		/* pup protocols: e.g. BSP */
#define	AF_CHAOS	5		/* mit CHAOS protocols */
#define	AF_NS		6		/* XEROX NS protocols */
#define	AF_ISO		7		/* ISO protocols */
#define	AF_OSI		AF_ISO
#define	AF_NBS		AF_ISO		/* IBM: nbs protocols */
					/* (hv: I think IBM is outdated here */
#define	AF_ECMA		8		/* european computer manufacturers */
#define	AF_DATAKIT	9		/* datakit protocols */
#define	AF_CCITT	10		/* CCITT protocols, X.25 etc */
#define	AF_SNA		11		/* IBM SNA */
#define AF_DECnet	12		/* DECnet */
#define AF_DLI		13		/* DEC Direct data link interface */
#define AF_LAT		14		/* LAT */
#define	AF_HYLINK	15		/* NSC Hyperchannel */
#define	AF_APPLETALK	16		/* Apple Talk */
#define	AF_NB		17		/* Netbios */
#define	AF_NETBIOS	AF_NB
#define	AF_MAX		18

#if 0		/* missing protocols */
#define	AF_ROUTE	17		/* Internal Routing Protocol */
#define	AF_LINK		18		/* Link layer interface */
#define	pseudo_AF_XTP	19		/* eXpress Transfer Protocol (no AF) */
#define	AF_COIP		20		/* connection-oriented IP, aka ST II */
#define	AF_CNT		21		/* Computer Network Technology */
#define pseudo_AF_RTIP	22		/* Help Identify RTIP packets */
#define	AF_IPX		23		/* Novell Internet Protocol */
#define	AF_INET6	24		/* IPv6 */
#define pseudo_AF_PIP	25		/* Help Identify PIP packets */
#define AF_ISDN		26		/* Integrated Services Digital Network*/
#define AF_E164		AF_ISDN		/* CCITT E.164 recommendation */
#define AF_NATM		27		/* native ATM access */
#define	AF_ENCAP	28
#define	AF_SIP		29		/* Simple Internet Protocol */
#define AF_KEY		30
#define	AF_MAX		31
#endif /* 0 */

/*
 * Structure used by kernel to store most
 * addresses.
 * is called struct osockaddr in 4.4BSD
 */
struct sockaddr {
	u_int8_t    sa_len;		/* sockaddr len including null; added */
	sa_family_t sa_family;		/* address family */
	char	sa_data[14];		/* up to 14 bytes of direct address */
};

/*
 * Sockaddr type which can hold any sockaddr type available
 * in the system.
 */
struct sockaddr_storage {
	u_int8_t    ss_len;		/* total length */
	sa_family_t ss_family;		/* address family */
	u_char	    __ss_pad1[6];	/* align to quad */
	u_int64_t   __ss_pad2;		/* force alignment for stupid compilers */
	u_char      __ss_pad3[240];	/* pad to a total of 256 bytes */
};

/*
 * Structure used by kernel to pass protocol
 * information in raw sockets.
 */
struct sockproto {
	u_short	sp_family;		/* address family */
	u_short	sp_protocol;		/* protocol */
};

/*
 * Protocol families, same as address families for now.
 */
#define	PF_UNSPEC	AF_UNSPEC
#define	PF_LOCAL	AF_LOCAL
#define	PF_UNIX		PF_LOCAL	/* backward compatibility */
#define	PF_OS2		AF_UNIX
#define	PF_INET		AF_INET
#define	PF_IMPLINK	AF_IMPLINK
#define	PF_PUP		AF_PUP
#define	PF_CHAOS	AF_CHAOS
#define	PF_NS		AF_NS
#define	PF_NBS		AF_NBS
#define	PF_ISO		AF_ISO
#define	PF_OSI		AF_ISO
#define	PF_ECMA		AF_ECMA
#define	PF_DATAKIT	AF_DATAKIT
#define	PF_CCITT	AF_CCITT
#define	PF_SNA		AF_SNA
#define PF_DECnet	AF_DECnet
#define PF_DLI		AF_DLI
#define PF_LAT		AF_LAT
#define	PF_HYLINK	AF_HYLINK
#define	PF_APPLETALK	AF_APPLETALK
#define	PF_NB		AF_NB
#define	PF_NETBIOS	AF_NB

#if 0 		/* not available */
#define	PF_ROUTE	AF_ROUTE
#define	PF_LINK		AF_LINK
#define	PF_XTP		pseudo_AF_XTP	/* really just proto family, no AF */
#define	PF_COIP		AF_COIP
#define	PF_CNT		AF_CNT
#define	PF_IPX		AF_IPX		/* same format as AF_NS */
#define PF_INET6	AF_INET6
#define PF_RTIP		pseudo_AF_FTIP	/* same format as AF_INET */
#define PF_PIP		pseudo_AF_PIP
#define PF_ISDN		AF_ISDN
#define PF_NATM		AF_NATM
#define PF_ENCAP	AF_ENCAP
#define	PF_SIP		AF_SIP
#define PF_KEY		AF_KEY
#endif /* 0 */

#define	PF_MAX		AF_MAX

/*
 * Socket credentials (unused, for now).
 */
struct sockcred {
	uid_t	sc_uid;			/* real user id */
	uid_t	sc_euid;		/* effective user id */
	gid_t	sc_gid;			/* real group id */
	gid_t	sc_egid;		/* effective group id */
	int	sc_ngroups;		/* number of supplemental groups */
	gid_t	sc_groups[1];		/* variable length */
};

/*
 * Compute size of a sockcred structure with groups.
 */
#define SOCKCREDSIZE(ngrps) \
	(sizeof(struct sockcred) + (sizeof(gid_t) * ((ngrps) - 1)))

/*
 * Maximum queue length specifiable by listen.
 */
#define	SOMAXCONN	5

/*
 * 4.3-compat message header (move to compat file later).
 * is called omsghdr in 4.4BSD
 */
struct msghdr {
	caddr_t		msg_name;	/* optional address */
	socklen_t	msg_namelen;	/* size of address */
	struct	iovec 	*msg_iov;	/* scatter/gather array */
	int		msg_iovlen;	/* # elements in msg_iov */
	caddr_t		msg_accrights;	/* access rights sent/received */
	int		msg_accrightslen;
	int		msg_flags;	/* flags on received message */
};

#define	MSG_OOB		0x1		/* process out-of-band data */
#define	MSG_PEEK	0x2		/* peek at incoming message */
#define	MSG_DONTROUTE	0x4		/* send without using routing tables */
#define	MSG_EOR		0x8		/* data completes record */
#define	MSG_TRUNC	0x10		/* data discarded before delivery */
#define	MSG_CTRUNC	0x20		/* control data lost before delivery */
#define	MSG_WAITALL	0x40		/* wait for full request or error */
#if 0 		/* not available */
#define	MSG_DONTWAIT	0x80		/* this message should be nonblocking */
#define	MSG_BCAST	0x100		/* this message rec'd as broadcast */
#define	MSG_MCAST	0x200		/* this message rec'd as multicast */
#endif /* 0 */

#define	MSG_MAXIOVLEN	16

/*
 * Header for ancillary data objects in msg_control buffer.
 * Used for additional information with/about a datagram
 * not expressible by flags.  The format is a sequence
 * of message elements headed by cmsghdr structures.
 */
struct cmsghdr {
	u_int	cmsg_len;		/* data byte count, including hdr */
	int	cmsg_level;		/* originating protocol */
	int	cmsg_type;		/* protocol-specific type */
/* followed by	u_char  cmsg_data[]; */
};

/* given pointer to struct cmsghdr, return pointer to data */
#define	CMSG_DATA(cmsg)		((u_char *)((cmsg) + 1))

/* given pointer to struct cmsghdr, return pointer to next cmsghdr */
#define	CMSG_NXTHDR(mhdr, cmsg)	\
	(((caddr_t)(cmsg) + (cmsg)->cmsg_len + sizeof(struct cmsghdr) > \
	    (mhdr)->msg_control + (mhdr)->msg_controllen) ? \
	    (struct cmsghdr *)NULL : \
	    (struct cmsghdr *)((caddr_t)(cmsg) + ALIGN((cmsg)->cmsg_len)))

#define	CMSG_FIRSTHDR(mhdr)	((struct cmsghdr *)(mhdr)->msg_control)

/* Round len up to next alignment boundary */
#define	CMSG_ALIGN(len)	(((len)+sizeof(long)-1) & ~(sizeof(long)-1))

/* Length of the contents of a control message of length len */
#define	CMSG_LEN(len)	(CMSG_ALIGN(sizeof(struct cmsghdr)) + (len))

/* Length of the space taken up by a padded control message of length len */
#define	CMSG_SPACE(len)	(CMSG_ALIGN(sizeof(struct cmsghdr)) + CMSG_ALIGN(len))

/* "Socket"-level control message types: */
#define	SCM_RIGHTS	0x01		/* access rights (array of int) */
#define SCM_CREDS	0x02		/* credientials (struct sockcred) */


/*
 * 4.3 compat sockaddr, move to compat file later
 */
struct osockaddr {
	u_short	sa_family;		/* address family */
	char	sa_data[14];		/* up to 14 bytes of direct address */
};

/*
 * 4.3-compat message header (move to compat file later).
 */
struct omsghdr {
	caddr_t	msg_name;		/* optional address */
	int	msg_namelen;		/* size of address */
	struct	iovec *msg_iov;		/* scatter/gather array */
	int	msg_iovlen;		/* # elements in msg_iov */
	caddr_t	msg_accrights;		/* access rights sent/received */
	int	msg_accrightslen;
};

#define SA_LEN(x) ((x)->sa_len)

extern int accept(int, struct sockaddr *, socklen_t *);
extern int bind(int, __const__ struct sockaddr *, socklen_t);
extern int connect(int,__const__ struct sockaddr*,socklen_t);
extern int gethostid(void);
extern int getpeername(int, struct sockaddr *, socklen_t *);
extern int getsockname(int, struct sockaddr *, socklen_t *);
extern int getsockopt(int, int, int, void *, socklen_t *);
extern int listen(int, int);
extern ssize_t	recv(int, void *, int, int);
extern ssize_t	recvfrom(int, void *, int, int, struct sockaddr *, socklen_t *);
extern	ssize_t	recvmsg(int, struct msghdr *, int);
extern	ssize_t	send(int, __const__ void *, int, int);
extern	ssize_t	sendto(int, __const__ void *, int, int, __const__ struct sockaddr*, socklen_t);
extern	ssize_t	sendmsg(int, __const__ struct msghdr *, int);
extern int setsockopt(int, int, int, __const__ void*, socklen_t);
extern int shutdown(int,int);
extern int socket(int,int,int);
extern int socketpair(int,int,int,int*);

extern	int	_impsockhandle(int, int);

/* special IBM prototypes 
 * hv: warning: usage of these routines may interfere with EMX operation
 * usage is not recommended.
 */
extern	void	psock_errno(__const__ char*);
extern	int	soabort(int);
extern	int	soclose(int);
extern	int	sock_init(void);
extern	int	sock_errno(void);
extern	int	so_cancel(int);
extern	int	getinetversion(char*);

/* hv: these are new TCPIPV4 data structures. Consult IBM doc on that */
#ifdef TCPIPV4
#define	MT_FREE		0
#define	MT_DATA		1
#define	MT_HEADER	2
#define	MT_SOCKET	3
#define	MT_PCB		4
#define	MT_RTABLE	5
#define	MT_HTABLE	6
#define	MT_ATABLE	7
#define	MT_SONAME	8
#define	MT_ZOMBIE	9
#define	MT_SOOPTS	10
#define	MT_FTABLE	11
#define	MT_RIGHTS	12
#define	MT_IFADDR	13

struct mbstat {
	u_short		m_mbufs;
	u_short		m_clusters;
	u_short		m_clfree;
	u_short		m_drops;
	u_long		m_wait;
	u_short		m_mtypes[256];
};

#define MAXSOCKETS	2048

struct sostats {
	short count;
	short socketdata[9*MAXSOCKETS];
};
#endif

#if defined (__cplusplus)
}
#endif

#endif /* !_SYS_SOCKET_H_ */
