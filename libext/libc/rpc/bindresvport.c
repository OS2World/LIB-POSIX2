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
 */

/*
 * Copyright (c) 1987 by Sun Microsystems, Inc.
 *
 * Portions Copyright(C) 1996, Jason Downs.  All rights reserved.
 */

#include <string.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*
 * Bind a socket to a privileged IP port
 */
int
bindresvport(sd, sadr_in)
	int sd;
	struct sockaddr_in *sadr_in;
{
	int on, old, error;
	struct sockaddr_in myaddr;
	int sinlen = sizeof(struct sockaddr_in);

	if (sadr_in == (struct sockaddr_in *)0) {
		sadr_in = &myaddr;
		memset(sadr_in, 0, sizeof(struct sockaddr_in));
		sadr_in->sin_len = sinlen;
		sadr_in->sin_family = AF_INET;
	} else if (sadr_in->sin_family != AF_INET) {
		errno = EPFNOSUPPORT;
		return (-1);
	}

	if (sadr_in->sin_port == 0) {
		int oldlen = sizeof(old);
		error = getsockopt(sd, IPPROTO_IP, IP_PORTRANGE,
				   &old, &oldlen);
		if (error < 0)
			return(error);

		on = IP_PORTRANGE_LOW;
		error = setsockopt(sd, IPPROTO_IP, IP_PORTRANGE,
		           	   &on, sizeof(on));
		if (error < 0)
			return(error);
	}

	error = bind(sd, (struct sockaddr *)sadr_in, sinlen);

	if (sadr_in->sin_port == 0) {
		int saved_errno = errno;

		if (error) {
			if (setsockopt(sd, IPPROTO_IP, IP_PORTRANGE,
			    &old, sizeof(old)) < 0)
				errno = saved_errno;
			return (error);
		}

		if (sadr_in != &myaddr) {
			/* Hmm, what did the kernel assign... */
			if (getsockname(sd, (struct sockaddr *)sadr_in,
			    &sinlen) < 0)
				errno = saved_errno;
			return (error);
		}
	}
	return (error);
}
