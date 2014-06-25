/* $Header: /cvsroot/posix2/posix2/libext/include/rpc/rpc.h,v 1.1 2000/11/17 19:14:59 arnd Exp $	*/

/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user.
 * 
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 * 
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 *
 *	from: @(#)rpc.h 1.9 88/02/08 SMI 
 *	@(#)rpc.h	2.4 89/07/11 4.0 RPCSRC
 */

/*
 * rpc.h, Just includes the billions of rpc header files necessary to
 * do remote procedure calling.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 */
#ifndef _RPC_RPC_H
#define _RPC_RPC_H

/* external data representation interfaces */
#include <rpc/xdr.h>		/* generic (de)serializer */

/* Client side only authentication */
#include <rpc/auth.h>		/* generic authenticator (client side) */

/* Client side (mostly) remote procedure call */
#include <rpc/clnt.h>		/* generic rpc stuff */

/* Client side (mostly) pmap functions */
#include <rpc/pmap_clnt.h>	/* generic pmap stuff */

/* semi-private protocol headers */
#include <rpc/rpc_msg.h>	/* protocol for rpc messages */
#include <rpc/auth_unix.h>	/* protocol for unix style cred */
/*
 *  Uncomment-out the next line if you are building the rpc library with    
 *  DES Authentication (see the README file in the secure_rpc/ directory).
 */
#if defined(__cplusplus)
# ifndef __inline__ 
#  define __inline__ inline
#  define __const__  const
# endif
extern "C" {
#endif


#if 0
#include <rpc/auth_des.h>	/* protocol for des style cred */
#endif

/* Server side only remote procedure callee */
#include <rpc/svc.h>		/* service manager and multiplexer */
#include <rpc/svc_auth.h>	/* service side authenticator */

/*
 * COMMENT OUT THE NEXT INCLUDE (or add to the #ifndef) IF RUNNING ON
 * A VERSION OF UNIX THAT USES SUN'S NFS SOURCE.  These systems will
 * already have the structures defined by <rpc/netdb.h> included in <netdb.h>.
 */
/* routines for parsing /etc/rpc */

struct rpcent {
      char    *r_name;        /* name of server for this rpc program */
      char    **r_aliases;    /* alias list */
      int     r_number;       /* rpc program number */
};

extern struct rpcent*getrpcbyname(__const__ char *);
extern struct rpcent*getrpcbynumber(int);
extern struct rpcent*getrpcent(void);
extern void setrpcent(int);
extern void endrpcent(void);

extern int get_myaddress(struct sockaddr_in *);
extern int registerrpc(int, int, int, char*(*)(char[UDPMSGSIZE]),
	xdrproc_t, xdrproc_t);
extern int callrpc(__const__ char *, int, int, int, xdrproc_t, __const__ char *,
	xdrproc_t, char *);
extern int getrpcport(__const__ char *, u_long,u_long,u_long);

#ifdef	__cplusplus
}
#endif

#endif /* !_RPC_RPC_H */