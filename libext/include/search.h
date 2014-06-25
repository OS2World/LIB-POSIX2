/* libext+gcc: search.h - $Id: search.h,v 1.2 2000/04/18 12:01:52 veit Exp $*/
/*
 * Written by J.T. Conklin <jtc@netbsd.org>
 * Public domain.
 */

#ifndef _SEARCH_H_
#define _SEARCH_H_

#include <stdlib.h>

#if defined (__cplusplus)
extern "C" {
#endif

typedef struct entry {
	char *key;
	char *data;
} ENTRY;

typedef enum {
	FIND, ENTER
} ACTION;

typedef enum {
	preorder,
	postorder,
	endorder,
	leaf
} VISIT;

extern void*bsearch(__const__ void *, __const__ void *, size_t, size_t,
			      int (*)(__const__ void *, __const__ void *));
extern int hcreate(unsigned int);
extern void hdestroy(void);
extern ENTRY*hsearch(ENTRY, ACTION);

extern void* lfind(__const__ void *, __const__ void *, size_t *, size_t,
			      int (*)(__const__ void *, __const__ void *));
extern void* lsearch(__const__ void *, __const__ void *, size_t *, size_t,
			      int (*)(__const__ void *, __const__ void *));
extern void insque(void *, void *);
extern void remque(void *);

extern void*tdelete(__const__ void *, void **,
			      int (*)(__const__ void *, __const__ void *));
extern void*tfind(__const__ void *, void * __const__ *,
			      int (*)(__const__ void *, __const__ void *));
extern void*tsearch(__const__ void *, void **, 
			      int (*)(__const__ void *, __const__ void *));
extern void twalk(__const__ void *, void (*)(__const__ void *, VISIT, int));

#if defined (__cplusplus)
}
#endif

#endif
