/* $Header: /cvsroot/posix2/posix2/libext/include/rpc/types.h,v 1.1 2000/11/17 19:14:59 arnd Exp $	*/

#ifndef _RPC_TYPES_H
#define _RPC_TYPES_H

#include <stdio.h>
#include <sys/types.h>

/*
 * Rpc additions to <sys/types.h>
 */
#define __dontcare__	-1

#ifndef FALSE
#	define FALSE	(0)
#endif
#ifndef TRUE
#	define TRUE	(1)
#endif
#ifndef NULL
#	define NULL	0
#endif

#define mem_alloc(bsize)	malloc(bsize)
#define mem_free(ptr, bsize)	free(ptr)

#define	bool_t	int32_t
#define	enum_t	int32_t

#endif