/* $Header: /cvsroot/posix2/posix2/libext/include/netinet/in.h,v 1.1 2000/11/17 19:14:59 arnd Exp $ */

/* Modified for emx by hv and em 1994,1996
 *
 * Copyright (c) 1982, 1986, 1990 Regents of the University of California.
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
 *	from: @(#)in.h	7.11 (Berkeley) 4/20/91
 *	$Id: in.h,v 1.1 2000/11/17 19:14:59 arnd Exp $
 */

#ifndef _NETINET_IN_H_
#define _NETINET_IN_H_

#ifndef _EMX_TCPIP
#define _EMX_TCPIP
#endif

#include <sys/param.h>          /* htons() etc. */

#if defined (__cplusplus)
extern "C" {
#endif

/*
 * Constants and structures defined by the internet system,
 * Per RFC 790, September 1981.
 */

/*
 * Protocols
 */
#define	IPPROTO_IP		0		/* dummy for IP */
#define	IPPROTO_ICMP		1		/* control message protocol */
#define	IPPROTO_IGMP		2		/* group mgmt protocol */
#define	IPPROTO_GGP		3		/* gateway^2 (deprecated) */
#define	IPPROTO_TCP		6		/* tcp */
#define	IPPROTO_EGP		8		/* exterior gateway protocol */
#define	IPPROTO_PUP		12		/* pup */
#define	IPPROTO_UDP		17		/* user datagram protocol */
#define	IPPROTO_IDP		22		/* xns idp */
#define	IPPROTO_TP		29 		/* tp-4 w/ class negotiation */
#define	IPPROTO_EON		80		/* ISO cnlp */

#define	IPPROTO_RAW		255		/* raw IP packet */
#define	IPPROTO_MAX		256


/*
 * From FreeBSD:
 *
 * Local port number conventions:
 *
 * When a user does a bind(2) or connect(2) with a port number of zero,
 * a non-conflicting local port address is chosen.
 * The default range is IPPORT_RESERVED through
 * IPPORT_USERRESERVED, although that is settable by sysctl.
 *
 * A user may set the IPPROTO_IP option IP_PORTRANGE to change this
 * default assignment range.
 *
 * The value IP_PORTRANGE_DEFAULT causes the default behavior.
 *
 * The value IP_PORTRANGE_HIGH changes the range of candidate port numbers
 * into the "high" range.  These are reserved for client outbound connections
 * which do not want to be filtered by any firewalls.
 *
 * The value IP_PORTRANGE_LOW changes the range to the "low" are
 * that is (by convention) restricted to privileged processes.  This
 * convention is based on "vouchsafe" principles only.  It is only secure
 * if you trust the remote host to restrict these ports.
 *
 * The default range of ports and the high range can be changed by
 * sysctl(3).  (net.inet.ip.port{hi}{first,last})
 *
 * Changing those values has bad security implications if you are
 * using a a stateless firewall that is allowing packets outside of that
 * range in order to allow transparent outgoing connections.
 *
 * Such a firewall configuration will generally depend on the use of these
 * default values.  If you change them, you may find your Security
 * Administrator looking for you with a heavy object.
 */

/*
 * Local port number conventions:
 * Ports < IPPORT_RESERVED are reserved for
 * privileged processes (e.g. root).
 * Ports > IPPORT_USERRESERVED are reserved
 * for servers, not necessarily privileged.
 */
#define	IPPORT_RESERVED		1024
#define	IPPORT_USERRESERVED	5000

/*
 * IP Version 4 Internet address (a structure for historical reasons)
 */
struct in_addr {
	in_addr_t s_addr;
};

/*
 * Definitions of bits in internet address integers.
 * On subnets, the decomposition of addresses to host and net parts
 * is done according to subnet mask, not the masks here.
 */
#define	IN_CLASSA(i)		(((long)(i) & 0x80000000) == 0)
#define	IN_CLASSA_NET		0xff000000
#define	IN_CLASSA_NSHIFT	24
#define	IN_CLASSA_HOST		0x00ffffff
#define	IN_CLASSA_MAX		128

#define	IN_CLASSB(i)		(((long)(i) & 0xc0000000) == 0x80000000)
#define	IN_CLASSB_NET		0xffff0000
#define	IN_CLASSB_NSHIFT	16
#define	IN_CLASSB_HOST		0x0000ffff
#define	IN_CLASSB_MAX		65536

#define	IN_CLASSC(i)		(((long)(i) & 0xe0000000) == 0xc0000000)
#define	IN_CLASSC_NET		0xffffff00
#define	IN_CLASSC_NSHIFT	8
#define	IN_CLASSC_HOST		0x000000ff

#define	IN_CLASSD(i)		(((long)(i) & 0xf0000000) == 0xe0000000)
#define	IN_CLASSD_NET		0xf0000000	/* These ones aren't really */
#define	IN_CLASSD_NSHIFT	28		/* net and host fields, but */
#define	IN_CLASSD_HOST		0x0fffffff	/* routing needn't know. */
#define	IN_MULTICAST(i)		IN_CLASSD(i)

#define	IN_EXPERIMENTAL(i)	(((long)(i) & 0xe0000000) == 0xe0000000)
#define	IN_BADCLASS(i)		(((long)(i) & 0xf0000000) == 0xf0000000)

#define	INADDR_ANY		(u_long)0x00000000
#define	INADDR_LOOPBACK		(u_long)0x7f000001
#define	INADDR_BROADCAST	(u_long)0xffffffff	/* must be masked */
#ifndef KERNEL
#define	INADDR_NONE		0xffffffff		/* -1 return */
#endif

#define	INADDR_UNSPEC_GROUP	(u_long)0xe0000000	/* 224.0.0.0 */
#define	INADDR_ALLHOSTS_GROUP	(u_long)0xe0000001	/* 224.0.0.1 */
#define	INADDR_MAX_LOCAL_GROUP	(u_long)0xe00000ff	/* 224.0.0.255 */

#define	IN_LOOPBACKNET		127			/* official! */

/*
 * Define a macro to stuff the loopback address into an Internet address
 */
#define	IN_SET_LOOPBACK_ADDR(a) { \
	(a)->sin_addr.s_addr = htonl(INADDR_LOOPBACK); \
	(a)->sin_family = AF_INET; }
/*
 * Socket address, internet style. 4.3BSD
 */
struct sockaddr_in {
	sa_family_t 	sin_family;
	in_port_t   	sin_port;
	struct in_addr 	sin_addr;
	int8_t		sin_zero[8];
	u_int8_t    	sin_len;	/* DUMMY: missing in emx (AH)!! */
};

/*
 * Structure used to describe IP options.
 * Used to store options internally, to pass them to a process,
 * or to restore options retrieved earlier.
 * The ip_dst is used for the first-hop gateway when using a source route
 * (this gets put into the header proper).
 */
struct ip_opts {
	struct	in_addr ip_dst;		/* first hop, 0 w/o src rt */
#if defined(__cplusplus)
	int8_t		Ip_opts[40];	/* cannot have same name as class */
#else
	int8_t		ip_opts[40];	/* actually variable in size */
#endif
};

/*
 * Options for use with [gs]etsockopt at the IP level.
 * First word of comment is data type; bool is stored in int.
 */
#define	IP_OPTIONS		1	/* buf/ip_opts; set/get IP options */

#define	IP_MULTICAST_IF		2	/* u_char; set/get IP mcast i/f */
#define	IP_MULTICAST_TTL	3	/* u_char; set/get IP mcast ttl */
#define	IP_MULTICAST_LOOP	4	/* u_char; set/get IP mcast loopback */
#define	IP_ADD_MEMBERSHIP	5	/* ip_mreq; add IP group membership */
#define	IP_DROP_MEMBERSHIP	6	/* ip_mreq; drop IP group membership */

#ifndef __EMX__ /* not supported so far */
#define	IP_HDRINCL		2	/* int; header is included with data */
#define	IP_TOS			3	/* int; IP type of service and preced */
#define	IP_TTL			4	/* int; IP time to live */
#define	IP_RECVOPTS		5	/* bool; receive all IP opts w/dgram */
#define	IP_RECVRETOPTS		6	/* bool; receive IP opts for response */
#define	IP_RECVDSTADDR		7	/* bool; receive IP dst addr w/dgram */
#define	IP_RETOPTS		8	/* ip_opts; set/get IP options */
#endif

/* 14-17 left empty for future compatibility with FreeBSD */

#define IP_PORTRANGE		19   /* int; range to choose for unspec port */


#define	IP_DEFAULT_MULTICAST_TTL   1    /* normally limit m'casts to 1 hop  */
#define	IP_DEFAULT_MULTICAST_LOOP  1    /* normally hear sends if a member  */
#define	IP_MAX_MEMBERSHIPS         20   /* per socket; must fit in one mbuf */

/*
 * Argument structure for IP_ADD_MEMBERSHIP and IP_DROP_MEMBERSHIP.
 */
struct ip_mreq {
	struct	in_addr imr_multiaddr;	/* IP multicast address of group */
	struct	in_addr imr_interface;	/* local IP address of interface */
};


/* dummies for emx !! (AH)
 * Argument for IP_PORTRANGE:
 * - which range to search when port is unspecified at bind() or connect()
 */
#define IP_PORTRANGE_DEFAULT	0	/* default range */
#define IP_PORTRANGE_HIGH	1	/* "high" - request firewall bypass */
#define IP_PORTRANGE_LOW	2	/* "low" - vouchsafe security */

extern	int		bindresvport(int, struct sockaddr_in *);

#if defined (__cplusplus)
}
#endif

#endif /* !_NETINET_IN_H_ */
